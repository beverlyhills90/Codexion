
# Codexion

## Description

Codexion is a concurrent simulation of programmers competing for shared USB dongles in a co-working hub. Each programmer is represented by an OS thread and must acquire two dongles simultaneously to compile their quantum code. The goal is to master thread synchronization — avoiding deadlocks, data races, starvation, and burnout — using POSIX threads, mutexes, and condition variables.

The simulation is a variation of the classic Dining Philosophers problem: coders sit in a circle, each sharing a dongle with their left and right neighbor. They cycle through three states — compiling, debugging, and refactoring. If a coder fails to start compiling within `time_to_burnout` milliseconds, they burn out and the simulation stops.

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

| Argument | Type | Description |
|---|---|---|
| `number_of_coders` | int | Number of coder threads and dongles |
| `time_to_burnout` | ms | Max time without compiling before burnout |
| `time_to_compile` | ms | Time spent compiling (holds 2 dongles) |
| `time_to_debug` | ms | Time spent debugging |
| `time_to_refactor` | ms | Time spent refactoring |
| `number_of_compiles_required` | int | Stop when all coders reach this count |
| `dongle_cooldown` | ms | Unavailability window after a dongle is released |
| `scheduler` | string | Arbitration policy: `fifo` or `edf` |

### Example

```bash
./codexion 5 1000 200 100 100 10 0 fifo
./codexion 5 1000 200 100 100 10 0 edf
```

## Blocking cases handled

### Deadlock prevention

The classic deadlock in dining philosophers occurs when every coder holds one dongle and waits for the other, creating a circular wait — one of Coffman's four necessary conditions. Codexion breaks this by enforcing a global acquisition order: coders always pick up their two dongles in ascending memory address order (`left < right` or `right < left` depending on the pointer comparison). This eliminates circular wait without any global lock.

### Starvation prevention

Under FIFO scheduling, each dongle maintains its own queue of waiting coders served strictly in arrival order. Under EDF (Earliest Deadline First), the coder whose deadline (`last_compile_start + time_to_burnout`) is closest gets priority. This prevents lower-indexed coders from monopolizing dongles at the expense of others.

### Cooldown handling

After a coder releases a dongle, the dongle records its release timestamp. Waiting coders use `pthread_cond_timedwait` with a timeout set to the cooldown expiry time. When they wake up — either from a `broadcast` or timeout — they re-check all conditions before proceeding. This avoids busy-waiting and ensures precise cooldown enforcement.

### Precise burnout detection

A dedicated monitor thread checks every coder's `time_from_last_compilation` every 5 ms. If `current_time - time_from_last_compilation >= time_to_burnout`, the monitor logs the burnout within 10 ms of the actual event, sets `is_running = STOP`, and broadcasts on all dongle condition variables to wake blocked coders so they can exit cleanly.

### Log serialization

All `printf` calls are wrapped in a dedicated `log_mutex`. This guarantees that state messages never interleave across lines, satisfying the requirement that each log line is atomic.

## Thread synchronization mechanisms

### Mutex inventory

| Mutex | Location | Protects |
|---|---|---|
| `world_mutex` | `t_world_data` | `is_running` flag — read by all threads, written by monitor |
| `coder_mutex` | `t_coder` | `time_from_last_compilation`, `number_of_compiles` — written by coder, read by monitor |
| `dongle_mutex` | `t_dongle` | `is_occupied`, `cooldown`, `queue` — shared between two adjacent coders |
| `log_mutex` | `t_world_data` | stdout — prevents interleaved log lines |

### Condition variables

Each dongle has a `pthread_cond_t`. When a coder wants a dongle that is occupied, on cooldown, or not their turn in the queue, it calls `pthread_cond_timedwait` — atomically releasing `dongle_mutex` and sleeping. The timeout is set to the dongle's cooldown expiry so the coder wakes automatically when cooldown ends, without any polling thread.

When a dongle is released, `giveup_dongle` calls `pthread_cond_broadcast` under `dongle_mutex`, waking all waiters. Each waiter re-checks the full condition:

```
is_occupied || get_ms() < cooldown || queue[0] != coder
```

Only the coder who is first in the queue and finds the dongle free proceeds; the rest sleep again.

### Race condition example

Without `coder_mutex`, the monitor thread could read `time_from_last_compilation` while the coder thread is writing it — a data race that could produce a false burnout. The mutex ensures these accesses are mutually exclusive:

- Coder locks `coder_mutex` → updates `time_from_last_compilation` → unlocks
- Monitor locks `coder_mutex` → reads `time_from_last_compilation` → unlocks

### Mutex and condition variable layout

```
┌─────────────────────────────────────────────────────────┐
│                    WORLD DATA                           │
│  [world_mutex] ── protects ──▶ is_running               │
│  [log_mutex]   ── protects ──▶ stdout                   │
└─────────────────────────────────────────────────────────┘

┌──────────────────┐        ┌──────────────────┐
│   CODER THREAD   │        │  MONITOR THREAD  │
│                  │        │                  │
│  [coder_mutex]   │        │  reads coder     │
│  ── protects ──▶ │        │  fields under    │
│  • compile_count │◀───────│  coder_mutex     │
│  • last_compile  │        │                  │
└──────────────────┘        └──────────────────┘

┌──────────────────────────────────────────────┐
│                  DONGLE                      │
│  [dongle_mutex] ── protects ──▶ is_occupied  │
│                             ──▶ cooldown     │
│                             ──▶ queue[]      │
│  [pthread_cond_t]                            │
│    coder ──▶ timedwait (sleeps + releases)   │
│    donor ──▶ broadcast (wakes all waiters)   │
└──────────────────────────────────────────────┘
```

### Thread-safe simulation stop

When burnout or completion is detected, the monitor:
1. Locks `world_mutex`, sets `is_running = STOP`, unlocks
2. For each dongle: locks `dongle_mutex`, broadcasts on its condition variable, unlocks

Coders sleeping in `pthread_cond_timedwait` wake up, re-check `is_running` under `world_mutex`, and exit their loop cleanly. The main thread then joins all coder threads and the monitor thread before freeing memory.

## Resources

- POSIX Threads Programming — Blaise Barney, Lawrence Livermore National Laboratory
- `man pthread_cond_timedwait`, `man pthread_mutex_lock`
- Helgrind user manual — Valgrind documentation

**AI usage**: Claude was used throughout this project for conceptual research and debugging guidance — understanding how mutexes and condition variables work, reasoning about deadlock conditions and README.md writing. All code was written by the student.

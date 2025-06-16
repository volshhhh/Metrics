# Система сбора и логирования метрик + детекция «роботов» по частоте кликов

## Описание  
Проект реализует многопоточную систему сбора, агрегации и логирования различных метрик (CPU, память, счётчик чисел, делящихся на 3) и отдельный модуль ClickTracker/RobotCount для детекции «роботов» по частоте кликов. Все метрики реализуют интерфейс `IMetric`, а логика записи вынесена в `MetricUtils/MetricsWriter`.

---


## Метрики (`ExampleMetrics`)

- **CounterMetric**  
  Считает, сколько случайных чисел, генерируемых в main.cpp, делятся на 3 (`considerNumber(int)`).

- **CpuPerformanceMetric**  
  Проводит синтетический бенчмарк CPU (количество операций sqrt/sin/cos за N ms).

- **MemoryUsageMetric**  
  Снимает текущее потребление памяти процесса (в МБ).

- **IMetric**  
  Базовый интерфейс:  
  ```cpp
  virtual const std::string& getName() const = 0;
  virtual std::string getValueAndReset() = 0;
  ```

---

## ClickTracker + RobotCount

- **ClickTracker** (`click/ClickTracker.h/.cpp`):  
  - Ведёт очередь временных меток кликов по пользователям.  
  - Параметры: `robotInterval` (ms), `robotClicks` (порог кликов за окно).  
  - При превышении порога банит пользователя и накапливает счётчик новых банов.  
  - Методы:  
    - `registerClick(userId)`  
    - `cntUsers()`, `cntRobots()`  
    - `getNewlyBannedRobotsAndReset()`

- **RobotCount** (`click/RobotCount.h/.cpp`):  
  `IMetric`-адаптер: в `getValueAndReset()` вызывает у `ClickTracker` `getNewlyBannedRobotsAndReset()`.

- **RobotCountTest** (`click/RobotCountTest.cpp`):  
  Симулирует `totalClicks` рандомных пачек кликов по `numUsers`, логирует число новых банов каждую секунду в `robots_detected_per_sec.log` (в корне).

---

## MetricsWriter (`MetricUtils`)

- Конструктор: `(filename, writeInterval_ms)`  
- `registerMetric(IMetric*)`  
- `start()` — запуск фона  
- Каждую секунду (или заданный интервал) опрашивает `getName() + getValueAndReset()` и пишет строку с timestamp:

  ```
  2025-06-16 12:00:01 "cpu_performance_score" 12345 "memory_usage_mb" 42 "divisible_by_3" 7
  ```

---

## Пример: `src/main.cpp`

```cpp
auto counter = std::make_unique<CounterMetric>("divisible_by_3");
auto cpuPerf = std::make_unique<CpuPerformanceMetric>("cpu_score", 100ms);
auto mem    = std::make_unique<MemoryUsageMetric>("memory_mb");

MetricsWriter writer("system_metrics.log", 1000ms);
writer.registerMetric(counter.get());
writer.registerMetric(cpuPerf.get());
writer.registerMetric(mem.get());
writer.start();

// генерация чисел, обновление counter->considerNumber(v);
// запуск синтетики cpuPerf->startMeasuring();
// sleep и выход через 10s
```

При запуске `my_app` в корне появится `system_metrics.log`.

---

## Пример: `click/RobotCountTest.cpp`

```cpp
auto tracker = make_shared<ClickTracker>(100ms, 5);
RobotCount robot("robots_detected_per_sec", tracker);
MetricsWriter writer("robots_detected_per_sec.log", 1000ms);
writer.registerMetric(&robot);
writer.start();

// генерируем 500 кликов пачками по 1–15 пользователей, sleep 40–60ms
// по завершении ждём ещё 2s и exit
```

В корне появится `robots_detected_per_sec.log`.

---

## Сборка и запуск

```bash
mkdir build
cmake -S . -B build # из корня
cmake --build build

# Запуск основной программы:
./build/my_app

# Запуск теста роботов:
./build/click/RobotCountLogTest 
```
---

## Настройка

- **Порог «робот»**: второй параметр `ClickTracker(robotInterval, robotClicks)`  
- **Интервал логирования**: в `MetricsWriter(filename, writeInterval_ms)`  

## Добавление новой метрики

1. Наследоваться от `IMetric`.  
2. Реализовать `getName()` и `getValueAndReset()`.  
3. Зарегистрировать метрику в `MetricsWriter` или `main.cpp`.  

--- 
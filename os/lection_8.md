ᅠ长闩 廾闩, [07.11.2025 22:53]
<!DOCTYPE html>
<html lang="ru">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Управление памятью в ОС: Слайды + Мемы</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap');
    body { margin: 0; padding: 0; font-family: 'Roboto', sans-serif; background: #0a1a2f; color: #fff; }
    .slide { width: 100vw; height: 100vh; display: flex; flex-direction: column; justify-content: center; align-items: center; padding: 20px; box-sizing: border-box; background: linear-gradient(135deg, #0a1a2f 0%, #1e3a5f 100%); position: relative; overflow: hidden; page-break-after: always; }
    .slide h1 { font-size: 2.5em; color: #00d4ff; text-align: center; margin: 10px 0; }
    .slide h2 { font-size: 2em; color: #ff8c00; margin: 10px 0; }
    .slide img { max-width: 90%; max-height: 60vh; border-radius: 12px; box-shadow: 0 0 20px rgba(0,212,255,0.3); margin: 10px; }
    .meme { max-width: 40%; border: 3px solid #ff8c00; border-radius: 12px; }
    .caption { font-size: 0.9em; color: #00d4ff; text-align: center; margin: 5px; }
    .footer { position: absolute; bottom: 10px; right: 20px; font-size: 0.8em; color: #00d4ff; }
    .center { text-align: center; }
  </style>
</head>
<body>

<!-- СЛАЙД 1: Титульный -->
<div class="slide center">
  <h1>Операционные системы</h1>
  <h2>#5 Подсистема управления памятью в ОС</h2>
  <img src="https://image.slidesharecdn.com/module-8-memory-management-powerpoint-ppt-presentation-160629120343/85/module-8-memory-management-1-320.jpg?cb=1467187485" alt="Титульный слайд: Иерархия памяти и ОС">
  <p class="caption">Источник: Slideserve - Memory Management PPT</p>
</div>

<!-- СЛАЙД 2: Простые схемы -->
<div class="slide center">
  <h2>ПРОСТЫЕ СХЕМЫ УПРАВЛЕНИЯ ПАМЯТЬЮ</h2>
  <img src="https://image.slidesharecdn.com/memory-managementinoperatingssystem-191003145052/85/memory-management-operating-system-1-320.jpg?cb=1570109455" alt="Девушка с попкорном и VR - метафора простых схем">
  <p class="caption">Визуал: Slideshare - Memory Management in OS</p>
</div>

<!-- СЛАЙД 3: Иерархия памяти -->
<div class="slide center">
  <h2>ИЕРАРХИЯ ПАМЯТИ</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-3-320.jpg?cb=1631862703" alt="Пирамида: Регистры → Кэш → RAM → Диск">
  <img src="https://programmerhumor.io/media/ram-meme-linux-vs-windows.jpg" class="meme" alt="Мем: Linux использует мало RAM, Windows - все">
  <p class="caption">Мем: ProgrammerHumor - RAM Memes</p>
</div>

<!-- СЛАЙД 4: Принцип локальности -->
<div class="slide">
  <h2>Принцип локальности</h2>
  <img src="https://image.slidesharecdn.com/i206s13lec18-130131220443-phpapp02/85/i206-lecture-18-memory-management-operating-systems-5-320.jpg?cb=1359653080" alt="Локальность: Программы работают с малым набором адресов">
  <img src="https://i.imgflip.com/8q0z0z.jpg" class="meme" alt="Мем: 'Не можешь доказать теорему? Прими за аксиому' - как локальность в мышлении">
  <p class="caption">Мем: Оригинальный стиль из PDF; похожий: Imgflip</p>
</div>

<!-- СЛАЙД 5: Проблема разрешения адресов -->
<div class="slide center">
  <h2>Проблема разрешения адресов</h2>
  <img src="https://image.slidesharecdn.com/453l8-110908093832-phpapp02/85/csc-453-operating-systems-lecture-8-memory-management-memory-is-a-resource-1-320.jpg?cb=1315460358" alt="Символьное → Логическое → Физическое пространство">
  <p class="caption">Источник: Adelphi.edu - CSC 453 Lecture</p>
</div>

<!-- СЛАЙД 6: Этапы выполнения -->
<div class="slide center">
  <h2>Этапы связывания адресов</h2>
  <img src="https://image.slidesharecdn.com/operating-system-3-memory-management-by-adi-pdf-210917105140/85/operating-system-3-memory-management-by-adi-5-320.jpg?cb=1631862703" alt="Диаграмма: Компиляция → Загрузка → Выполнение">
  <p class="caption">Источник: Slideshare - OS Memory Management</p>
</div>

ᅠ长闩 廾闩, [07.11.2025 22:53]
<!-- СЛАЙД 7: Проблема связывания -->
<div class="slide">
  <h2>Проблема связывания адресов</h2>
  <img src="https://image.slidesharecdn.com/unit-4memory-management-operatingsystemspptx-211202170051/85/unit-4-memory-management-operating-systems-3-320.jpg?cb=1638455256" alt="Этапы: Компиляция, Загрузка, Выполнение">
  <img src="https://programmerhumor.io/media/memory-leak-meme.jpg" class="meme" alt="Мем: 'Я вам посылку принёс, но не отдам - документов нет' - как миграция образов">
  <p class="caption">Мем: ProgrammerHumor - Memory Management Memes</p>
</div>

<!-- СЛАЙД 8: Логическое адресное пространство -->
<div class="slide center">
  <h2>Логическое адресное пространство</h2>
  <img src="https://image.slidesharecdn.com/memory-management-13175502/85/memory-management-13175502-10-320.jpg?cb=1334466128" alt="Символьное, Логическое, Физическое АП">
  <p class="caption">Источник: Slideshare - Memory Management</p>
</div>

<!-- СЛАЙД 9: Функции ОС и hardware -->
<div class="slide center">
  <h2>Функции ОС и hardware</h2>
  <img src="https://image.slidesharecdn.com/operating-systems-memory-management-72943904/85/operating-systems-memory-management-14-320.jpg?cb=1481249989" alt="Отображение, Распределение, Контроль, Выгрузка, Учёт">
  <p class="caption">Источник: Slideshare - OS Memory Management</p>
</div>

<!-- СЛАЙД 10: Однопрограммная система -->
<div class="slide center">
  <h2>Однопрограммная система</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-6-320.jpg?cb=1631862703" alt="0 → ОС → Процесс → max">
  <p class="caption">Источник: Slideshare - Galvin OS Concepts</p>
</div>

<!-- СЛАЙД 11: Фиксированные разделы -->
<div class="slide center">
  <h2>Фиксированные разделы</h2>
  <img src="https://image.slidesharecdn.com/unit-4memory-management-operatingsystemspptx-211202170051/85/unit-4-memory-management-operating-systems-9-320.jpg?cb=1638455256" alt="ОС + Разделы 1-3 + Очереди">
  <p class="caption">Источник: Slideshare - Unit 4 Memory Management</p>
</div>

<!-- СЛАЙД 12: Организация больших программ -->
<div class="slide center">
  <h2>Организация больших программ</h2>
  <img src="https://image.slidesharecdn.com/memory-managementinoperatingssystem-191003145052/85/memory-management-operating-system-12-320.jpg?cb=1570109455" alt="Оверлеи и динамическая загрузка">
  <p class="caption">Источник: Slideshare - Memory in OS</p>
</div>

<!-- СЛАЙД 13: Динамические разделы (с фрагментацией) -->
<div class="slide">
  <h2>Динамические разделы</h2>
  <img src="https://image.slidesharecdn.com/operating-system-3-memory-management-by-adi-pdf-210917105140/85/operating-system-3-memory-management-by-adi-15-320.jpg?cb=1631862703" alt="P1, P2, P3 с фрагментацией">
  <img src="https://programmerhumor.io/media/fragmentation-meme.jpg" class="meme" alt="Мем: Фрагментация как нарезка свободных мест">
  <p class="caption">Мем: ProgrammerHumor - Memory Memes</p>
</div>

<!-- СЛАЙД 14: Сборщик мусора -->
<div class="slide center">
  <h2>Динамические разделы: Фрагментация</h2>
  <img src="https://image.slidesharecdn.com/memory-management-65585423/85/memory-management-17-320.jpg?cb=1475671234" alt="Сборщик мусора и дефрагментация">
  <p class="caption">Источник: Slideshare - Memory Management</p>
</div>

<!-- СЛАЙД 15: БУП / MMU -->
<div class="slide center">
  <h2>БУП / MMU</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-11-320.jpg?cb=1631862703" alt="Логический + Сегмент → Физический">
  <p class="caption">Источник: Slideshare - Galvin Ch8</p>
</div>

<!-- СЛАЙД 16: Сегментная организация -->
<div class="slide center">
  <h2>Сегментная организация</h2>
  <img src="https://image.slidesharecdn.com/operating-system-3-memory-management-by-adi-pdf-210917105140/85/operating-system-3-memory-management-by-adi-25-320.jpg?cb=1631862703" alt="(Nseg, offset) → Таблица сегментов">
  <p class="caption">Источник: Slideshare - OS-3 Memory</p>
</div>

ᅠ长闩 廾闩, [07.11.2025 22:53]
<!-- СЛАЙД 17: Страничная организация -->
<div class="slide center">
  <h2>Страничная организация</h2>
  <img src="https://image.slidesharecdn.com/memory-management-254405163/85/memory-management-21-320.jpg?cb=1699089872" alt="Pages → Frames, Таблица страниц">
  <p class="caption">Источник: Slideshare - Memory Management 2023</p>
</div>

<!-- СЛАЙД 18: Сегментно-страничная -->
<div class="slide center">
  <h2>Сегментно-страничная</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-19-320.jpg?cb=1631862703" alt="(Nseg, Npage, offset) → Nframe">
  <p class="caption">Источник: Slideshare - Galvin</p>
</div>

<!-- СЛАЙД 19: Многоуровневая таблица -->
<div class="slide center">
  <h2>Многоуровневая таблица страниц</h2>
  <img src="https://image.slidesharecdn.com/module-8-memory-management-powerpoint-ppt-presentation-160629120343/85/module-8-memory-management-25-320.jpg?cb=1467187485" alt="Двухуровневая таблица">
  <p class="caption">Источник: Slideserve - Module 8</p>
</div>

<!-- СЛАЙД 20: TLB -->
<div class="slide center">
  <h2>TLB (Ассоциативная память)</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-27-320.jpg?cb=1631862703" alt="Page + offset → TLB → Frame">
  <p class="caption">Источник: Slideshare - Galvin</p>
</div>

<!-- СЛАЙД 21: Виртуальная память -->
<div class="slide center">
  <h2>ВИРТУАЛЬНАЯ ПАМЯТЬ</h2>
  <img src="https://image.slidesharecdn.com/memory-managementinoperatingssystem-191003145052/85/memory-management-operating-system-35-320.jpg?cb=1570109455" alt="Виртуальное АП → Физическое">
  <p class="caption">Источник: Slideshare - Memory in OS</p>
</div>

<!-- СЛАЙД 22: Концепция виртуальной памяти -->
<div class="slide center">
  <h2>Концепция виртуальной памяти</h2>
  <img src="https://image.slidesharecdn.com/operating-system-3-memory-management-by-adi-pdf-210917105140/85/operating-system-3-memory-management-by-adi-35-320.jpg?cb=1631862703" alt="Подкачка страниц">
  <p class="caption">Источник: Slideshare - OS-3</p>
</div>

<!-- СЛАЙД 23: Преимущества -->
<div class="slide">
  <h2>Преимущества виртуальной памяти</h2>
  <img src="https://image.slidesharecdn.com/unit-4memory-management-operatingsystemspptx-211202170051/85/unit-4-memory-management-operating-systems-40-320.jpg?cb=1638455256" alt="Не ограничен RAM, Мультипрограммирование">
  <img src="https://programmerhumor.io/media/virtual-memory-meme.jpg" class="meme" alt="Мем: 'Download more RAM' - классика виртуальной памяти">
  <p class="caption">Мем: ProgrammerHumor - RAM Memes</p>
</div>

<!-- СЛАЙД 24: Page fault -->
<div class="slide">
  <h2>Page fault</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-31-320.jpg?cb=1631862703" alt="Page fault: Hardware + Software обработка">
  <img src="https://i.imgflip.com/8q0z0z.jpg" class="meme" alt="Мем: Page fault как 'fault' в NPC - Imgflip">
  <p class="caption">Мем: Imgflip - Angry NPC Page Fault</p>
</div>

<!-- СЛАЙД 25: Стратегии -->
<div class="slide center">
  <h2>Стратегии управления</h2>
  <img src="https://image.slidesharecdn.com/memory-management-65585423/85/memory-management-40-320.jpg?cb=1475671234" alt="Выборка, Размещение, Замещение">
  <p class="caption">Источник: Slideshare - Memory Management</p>
</div>

<!-- СЛАЙД 26: Алгоритмы замещения -->
<div class="slide center">
  <h2>Алгоритмы замещения страниц</h2>
  <img src="https://image.slidesharecdn.com/module-8-memory-management-powerpoint-ppt-presentation-160629120343/85/module-8-memory-management-45-320.jpg?cb=1467187485" alt="FIFO, OPT, LRU">
  <p class="caption">Источник: Slideserve - Module 8</p>
</div>

ᅠ长闩 廾闩, [07.11.2025 22:53]
<!-- СЛАЙД 27: Локальные алгоритмы -->
<div class="slide center">
  <h2>Локальные алгоритмы</h2>
  <img src="https://image.slidesharecdn.com/operating-system-3-memory-management-by-adi-pdf-210917105140/85/operating-system-3-memory-management-by-adi-50-320.jpg?cb=1631862703" alt="FIFO с аномалией Belady">
  <p class="caption">Источник: Slideshare - OS-3</p>
</div>

<!-- СЛАЙД 28: Глобальные + Трешинг -->
<div class="slide">
  <h2>Глобальные алгоритмы: Трешинг</h2>
  <img src="https://image.slidesharecdn.com/ch8pdf-operatingsystemsbygalvintolearn-210917105140/85/ch8pdf-operating-systems-by-galvin-to-learn-55-320.jpg?cb=1631862703" alt="Thrashing: Page faults > Выполнение">
  <img src="https://programmerhumor.io/media/thrashing-meme.jpg" class="meme" alt="Мем: Thrashing как 'unused RAM is wasted' - но наоборот">
  <p class="caption">Мем: Reddit/LinuxMemes - Thrashing Experiment</p>
</div>

<!-- СЛАЙД 29: Алгоритм границ -->
<div class="slide center">
  <h2>Алгоритм границ</h2>
  <img src="https://image.slidesharecdn.com/memory-managementinoperatingssystem-191003145052/85/memory-management-operating-system-60-320.jpg?cb=1570109455" alt="Верхняя/Нижняя граница page faults">
  <p class="caption">Источник: Slideshare - Memory in OS</p>
</div>

<!-- СЛАЙД 30: Спасибо -->
<div class="slide center">
  <h1>Спасибо за внимание!</h1>
  <img src="https://programmerhumor.io/media/final-memory-meme.jpg" class="meme" alt="Мем: 'Memory management evolved to GC' - финальный юмор">
  <p class="caption">Мем: ProgrammerHumor - How Memory Evolved</p>
</div>

<div class="footer">Источники: Slideshare, ProgrammerHumor, Imgflip | © 2025</div>
</body>
</html>

ᅠ长闩 廾闩, [07.11.2025 23:02]
- Уровень K → использует K-1, предоставляет K+1  
- Нельзя пропускать уровни  
- Вертикальные интерфейсы — контроль  
- Горизонтальные — договорённости (язык, формат)

---

## Модель OSI/ISO  

| Уровень | Название | Функция |
|--------|---------|--------|
| 1 | Физический | Биты |
| 2 | Канальный | Пакеты |
| 3 | Сетевой | Доставка узел → узел |
| 4 | Транспортный | Процесс → процесс |
| 5 | Сеансовый | Сеанс, контрольные точки |
| 6 | Представления | Преобразование данных |
| 7 | Приложений | Интерфейс |

---

## Адресация  

### Одноуровневая  
1. Процесс → ОС  
2. ОС опрашивает все другие ОС  
3. Принимает решение  
4. Извещает процесс  
> Только для малых сетей

### Двухуровневая (DNS)  
- Деление на ранги → data.itech.ibisedu.ru  
- Сервер DNS в каждом ранге >1  
- Кэширование → быстрое разрешение  

### Локальные и полные адреса  
- Порт вместо PID  
- Сокет = <числовой адрес, порт>

---

## Маршрутизация  

| Тип | Описание |
|-----|---------|
| От источника | Путь прописан явно |
| Одношаговая | Узлы решают сами |

### Алгоритмы одношаговой:  
- Фиксированная — таблица маршрутов  
- Лавинная — надёжно, но трафик  
- Случайная — проще  
- По прецедентам — заполняется в работе  
- Векторно-дистанционная — метрика переходов  
- Состояния связей — веса, обмен пакетами  

---

## Интерфейсы  

| Тип | Описание |
|-----|---------|
| Connectionless | Датаграммы |
| Connection-oriented | Логическое соединение |

---

## Спасибо за внимание!  

![Финал](https://programmerhumor.io/media/final-memory-meme.jpg)  
*Мем: "How memory management evolved to GC"*

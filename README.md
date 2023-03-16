Сербин Георгий Эдуардович (Telegram: @GoshaSerbin)
ML-12
Ментор: Арсений Евдокимов (Telegram: @Arugaf)

Домашнее задание 1:

Работа с файлами и строками

Дедлайн 28 марта в 18:00

За это ДЗ — +10 баллов "Выполнение задания"

В домашнем задании необходимо написать код программы, выполняющую определенную задачу в зависимости от варианта

Вариант #10

Необходимо разработать программу, которая обрабатывает данные онлайн сервиса MusicBrainz.
Датасет можно скачать на официальном сайте MusicBrainz https://data.metabrainz.org/pub/musicbrainz/data/fullexport/
Необходимый файл mbdump.tar.bz2.
Описание датасета находится по ссылке https://musicbrainz.org/doc/MusicBrainz_Database/Schema
Например, для Артиста диаграмма находится по ссылке https://wiki.musicbrainz.org/images/7/7e/artist_entity_details.svg
Файл датасета представляет из себя сжатый архив из множества текстовых файлов, где данные располагаются в строчках, а поля разделены символом табуляции \t
Каждая таблица на схемах представлена отдельным файлом с аналогичным названием

Скачать датасет по ссылке можно с помощью команды wget, например
wget https://data.metabrainz.org/pub/musicbrainz/data/fullexport/20230304-002037/mbdump.tar.bz2
Полученный файл нужно распаковать
tar xvf mbdump.tar.bz2
Все файлы кроме gender, area, artist, artist_type можно удалить с помощью команды rm.
Программа должна принимать необходимые для работы имена распакованных файлов в качестве аргументов командной строки и выводить данные в стандартный вывод.
Программа не должна использовать интерактивный ввод с клавиатуры, например, "введите имя файла", "введите необходимый год", "ведите q чтобы выйти" - такими программами не удобно пользоваться и их разработка занимает больше времени.

Ваш вариант:
Вывести пол, и дату рождения (при наличии) исполнителя, имя которого переданно в аргументах командной строки
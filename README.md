# Тестовое задание
## Программа 1 реализована на python
Для установки библиотек и зависимостей используется poetry
```console
sudo apt install python3-poetry
poetry lock
poetry install
poetry run python ipcheckertest/main.py
```
Чтобы работал захват пакетов, необходимы разрешения, для этого запускать нужно с помощью sudo
```console
sudo poetry install
sudo poetry run python ipcheckertest/main.py
```
## Программа 2 реализована на C++
Для сборки можно использовать g++
```console
g++ CProgramm/main.cpp
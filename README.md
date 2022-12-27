Перед використанням програми згенеруйте файл конфігурації. Запустить cvd-conf.sh для цього, якщо користуєтесь лінуксом. Конфіг можна буде знайти за шляхом ~/.config/cvd/cvd.conf

Якщо у вас встановлена ОС Windows, то файл конфігурації знаходиться за шляхом %appdata%/cvd/cvd.conf. Конфігурацію потрібно написати самостійно, наприклад
```
libriamirror=https://anilibria.tv/public/torrent/download.php?id=
torrserver=127.0.0.1:8090 — адреса torrserver
limit=50 — ліміт тайтлів за один пошук
player=C:\ProgramData\chocolatey\lib\mpv.install\tools\mpv.exe — шлях до плеєра
torrent="C:\Program Files\qBittorrent\qbittorrent.exe" — шлях до торент-клієнта
iregretnothing=1 — шукати приховані релізи
monsterres=2 — якість відео плеєру "МОНСТР", 0 - 360p, 1 - 720p, 2 - 1080p
confverA=0
confverB=2
confverC=0
```
за відсутністю, наприклад, torrserver чи будь якого іншого компонента його слід не записувати у файл конфігурації.

Задля компіляції програми необхідно мати [cpp-httplib](https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h)

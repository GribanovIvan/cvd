#!/bin/bash
printf 'Введите максимальное количество релизов, которые будут показаны за раз, -1 - без ограничений (50): '
read limit
if [ "$limit" = '' ]; then limit=50;fi
echo Выберите желаемый плеер для видео:
i=0; for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | grep -v \: | grep -v mediainfo | sed s/'      4 '//g | sed s/.desktop//g); do let 'i = i + 1'; echo $i\) $f;done
mpv=$(xdg-mime query default video/mp4 | sed s/.desktop//g)
if [ "$(type "$mpv" 2>/dev/null)" = "" ]; then echo problem;else echo ok;fi
printf \($mpv\):\ 
read player
if [ "$player" = '' ]; then player=$mpv;
else
all=$(for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | grep -v \: | sed s/'      4 '//g); do echo $f;done | wc -l)
if [ "$player" -le "$all" ] && [ "$player" -gt 0 ]; then player=$(grep Exec /usr/share/applications/$(i=0; for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | sed s/'      4 '//g); do echo $f;done | cut -d'
' -f$player)| cut -d'
' -f1 | cut -c9-)
fi 2>/dev/null;fi
#if [ "$player" = mpv ];then player="mpv --write-filename-in-watch-later-config --watch-later-directory=~/config/cvd/mpv_watch_later";fi
echo Введите адрес torrserver, если таковой имеется
if [[ $(curl 127.0.0.1:8090 2>/dev/null) = *torrserver* ]];then mpv=127.0.0.1:8090;printf \($mpv\):\ ;fi
read torrserver
if [ "$torrserver" = '' ] && [ "$mpv" = 127.0.0.1:8090 ]; then torrserver=127.0.0.1:8090
else
until [[ "$torrserver" = *.*.*.*:* ]]; do echo Неверный формат.;read torrserver;done
fi
echo Выберите желаемый торрент-клиент:
i=0; for f in $(sh -c 'gio mime x-scheme-handler/magnet;gio mime application/x-bittorrent' | sed s/org.qbittorrent.//g | sort | uniq -cd | grep -v \: | grep -v mediainfo | sed s/'      4 '//g | sed s/.desktop//g); do let 'i = i + 1'; echo $i\) $f;done
mpv=$(xdg-mime query default application/x-bittorrent | sed s/.desktop//g)
if [ "$(type "$mpv" 2>/dev/null)" = "" ]; then mpv=$(grep Exec /usr/share/applications/$(xdg-mime query default application/x-bittorrent) | cut -d'
' -f1 | sed s/TryExec=//g | sed s/Exec=//g | sed s/' %U'//g);fi
printf \($mpv\):\ 
read torrent
if [ "$torrent" = '' ]; then torrent=$mpv;
else
all=$(for f in $(sh -c 'gio mime x-scheme-handler/magnet4;gio mime application/x-bittorrent' | sort | uniq -cd | grep -v \: | sed s/'      4 '//g); do echo $f;done | wc -l)
if [ "$torrent" -le "$all" ] && [ "$torrent" -gt 0 ]; then torrent=$(grep Exec /usr/share/applications/$(i=0; for f in $(sh -c 'gio mime x-scheme-handler/magnet4;gio mime application/x-bittorrent' | sort | uniq -cd | sed s/'      4 '//g); do echo $f;done | cut -d'
' -f$torrent)| cut -d'
' -f1 | cut -c9-)
fi 2>/dev/null;fi
darklink=$(curl 2>/dev/null https://darklibria.it/search?find=senko | grep register_click | grep btn | sed s/.*href=\"//g | cut -d\" -f1 | sed s/'release\/sewayaki-kitsune-no-senko-san'/'upload\/torrents\/'/g)
if [ $(echo "$(curl -w %{time_total} -o /dev/null -s https://anilibria.tv/public/torrent) > $(curl -w %{time_total} -o /dev/null -s $darklink)" | bc) -eq 1 ]; then
echo libriamirror=https://anilibria.tv/public/torrent/download.php?id= > ~/.config/cvd/cvd.conf;else echo "darklike=.torrent
libriamirror=$darklink" > ~/.config/cvd/cvd.conf;fi
echo "limit=$limit
player=$player
torrserver=$torrserver
torrent=$torrent
confverA=0
confverB=0" >> ~/.config/cvd/cvd.conf

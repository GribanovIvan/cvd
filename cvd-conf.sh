#!/bin/bash
limistr='Enter max amount of titels what can be displayed at time. -1 - no limit'
choose='Choose videoplayer'
choose2='Enter torrserver address if there is'
choose3='Choose torrent-client'
choose4='Show hidden content? (ONLY FOR NON-PROFIT USE)'
choose5='Choose desirable resolution for player, for example 1080 or 720'
ni='no'
yea='yes'
warns='Display warnings?'
overs='Always overwrite playlist filename by'
login='Login to anitube.in.ua. Or leave blank'
murmur='Search on anitube.in.ua instead of anilibria.tv'
crypted='password will be crypted!'
display='Display title name once more'
autoexpand='Automaticly expand anitube.in.ua results if it is more than 5'
if [ $(echo $LANG | cut -d_ -f1) = ru ] || [ $(echo $LANG | cut -d_ -f1) = uk ]; then limistr='Введіть максимальну кількість релізів, які будуть відображені одночасно. -1 - без обмеження'
choose='Виберіть бажаний плеєр для відео'
choose2='Введіть адресу torrserver, якщо така є'
choose3='Виберіть бажаний торент-клієнт'
choose4='Виконувати пошук по прихованим джерелам? (ВИКЛЮЧНО ДЛЯ ОЗНАЙОМЛЕННЯ, ЗАБОРОНЕНО КОМЕРЦІЙНЕ ВИКОРИСТАННЯ)'
choose5='Введіть бажану якість плеєра, наприклад, 1080 чи 720'
ni='ні'
yea='так'
warns='Відображати попередження?'
overs='Завжди зберігати плейліст, як'
login='Увійдіть до anitube.in.ua. Якщо ви не маєте акаунту, то просто натисність Enter'
murmur='Шукати на anitube.in.ua замість anilibria.tv'
crypted='пароль буде зашифровано'
display='Відображати назву тайтлу зайвий раз'
autoexpand='Автоматично розкривати результати пошуку на anitube.in.ua, якщо їх більше 5'
fi
defaultlim=$(($(stty size | cut -d' ' -f1)-1))
printf "$limistr"' ('"$defaultlim"'): '
read limit
if [ "$limit" = '' ]; then limit=$defaultlim;fi
echo $choose:
if [ -d /data/data/com.termux ]; then echo Not implemented yet :\(; else
i=0; for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | grep -v \: | grep -v mediainfo | grep -v audacity | grep -vi handbrake | grep -v kate | sed s/'      4 '//g | sed s/'      2 '//g); do let 'i = i + 1'; echo $i\) $(toecho="$(grep Name'\['$(echo $LANG | cut -d_ -f1) /usr/share/applications/$f | grep -v GenericName | cut -d= -f2)";if [ "$toecho" = '' ];then echo $(grep Name= /usr/share/applications/$f | grep -v GenericName | cut -d= -f2) ;else echo $toecho;fi);done
mpv=$(xdg-mime query default video/mp4 | sed s/.desktop//g)
if [ "$(type "$mpv" 2>/dev/null)" = "" ]; then mpv=$(grep Exec /usr/share/applications/$(xdg-mime query default xdg-mime query default video/mp4) | cut -d'
' -f1 | sed s/TryExec=//g | sed s/Exec=//g | sed s/' %U'//g);fi;fi
printf \($mpv\):\ 
read player
if [ "$player" = '' ]; then player=$mpv
else
all=$(for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | grep -v \: | sed s/'      4 '//g); do echo $f;done | wc -l)
if [ "$player" -le "$all" ] && [ "$player" -gt 0 ]; then player=$(grep Exec /usr/share/applications/$(i=0; for f in $(sh -c 'gio mime video/mp4;gio mime audio/x-mpegurl' | sort | uniq -cd | sed s/'      4 '//g); do echo $f;done | cut -d'
' -f$player)| cut -d'
' -f1 | cut -c9-)
fi 2>/dev/null;fi
#if [ "$player" = mpv ];then player="mpv --write-filename-in-watch-later-config --watch-later-directory=~/config/cvd/mpv_watch_later";fi
echo $choose2
if [[ $(curl 127.0.0.1:8090 2>/dev/null) = *torrserver* ]];then mpv=127.0.0.1:8090;printf \($mpv\):\ ;fi
read torrserver
if [ "$torrserver" = '' ] && [ "$mpv" = 127.0.0.1:8090 ]; then torrserver=127.0.0.1:8090
elif [ "$torrserver" = '' ]; then torrserver=....:none; else
until [[ "$torrserver" = *.*.*.*:* ]]; do echo Неверный формат.;read torrserver;done
fi
echo $choose3:
i=0; for f in $(sh -c 'gio mime x-scheme-handler/magnet;gio mime application/x-bittorrent' |  sort | uniq -cd | grep -v \:  | sed s/'      4 '//g); do let 'i = i + 1'; echo $i\) $(grep Name'\['$(echo $LANG | cut -d_ -f1) /usr/share/applications/$f | grep -v GenericName | cut -d= -f2);done
mpv=$(xdg-mime query default application/x-bittorrent | sed s/.desktop//g)
if [ "$(type "$mpv" 2>/dev/null)" = "" ]; then mpv=$(grep Exec /usr/share/applications/$(xdg-mime query default application/x-bittorrent) 2>/dev/null | cut -d'
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
echo "$choose4"
printf \($ni\)\:\ 
until [ "$iregretnothing" = 0 ] || [ "$iregretnothing" = 1 ]; do read iregretnothing
if [[ ${iregretnothing,,} = *y* ]] || [[ ${iregretnothing,,} = *д* ]] || [[ ${iregretnothing,,} = *т* ]]; then iregretnothing=1;
elif [[ ${iregretnothing,,} = *n* ]] || [[ ${iregretnothing,,} = *н* ]] || [[ "$iregretnothing" = '' ]]; then iregretnothing=0;fi;done
echo "$warns"
printf \($yea\)\:\ 
until [ "$warn" = 0 ] || [ "$warn" = 1 ]; do read warn
if [[ ${warn,,} = *y* ]] || [[ ${warn,,} = *д* ]] || [[ ${warn,,} = *т* ]] || [[ "$warn" = '' ]]; then warn=1;
elif [[ ${warn,,} = *n* ]] || [[ ${warn,,} = *н* ]]; then warn=0;fi;done
echo $choose5
printf \(1080\):\ 
read res
if [ "$res" = "" ];then res=1080;fi
echo "$overs" cvd.m3u?
printf \($ni\)\:\ 
until [ "$over" = 0 ] || [ "$over" = 1 ]; do read over
if [[ ${over,,} = *y* ]] || [[ ${over,,} = *д* ]] || [[ ${over,,} = *т* ]]; then over=1;
elif [[ ${over,,} = *n* ]] || [[ ${over,,} = *н* ]] || [[ "$over" = '' ]]; then over=0;fi;done
echo $murmur
printf \($yea\)\:\ 
until [ "$norusso" = 0 ] || [ "$norusso" = 1 ]; do read norusso
if [[ ${norusso,,} = *y* ]] || [[ ${norusso,,} = *д* ]] || [[ ${norusso,,} = *т* ]] || [[ "$norusso" = '' ]]; then norusso=1;
elif [[ ${norusso,,} = *n* ]] || [[ ${norusso,,} = *н* ]]; then norusso=0;fi;done
echo $display?
printf \($yea\)\:\ 
until [ "$display" = 0 ] || [ "$display" = 1 ]; do read display
if [[ ${display,,} = *y* ]] || [[ ${display,,} = *д* ]] || [[ ${display,,} = *т* ]] || [[ "$display" = '' ]]; then display=1;
elif [[ ${display,,} = *n* ]] || [[ ${display,,} = *н* ]]; then display=0;fi;done
echo $autoexpand?
printf \($yea\)\:\ 
until [ "$autoexpand" = 0 ] || [ "$autoexpand" = 1 ]; do read autoexpand
if [[ ${autoexpand,,} = *y* ]] || [[ ${autoexpand,,} = *д* ]] || [[ ${autoexpand,,} = *т* ]] || [[ "$autoexpand" = '' ]]; then autoexpand=1;
elif [[ ${autoexpand,,} = *n* ]] || [[ ${autoexpand,,} = *н* ]]; then autoexpand=0;fi;done
echo -e $login "\e[32m"\($crypted\)"\e[0m"
printf 'login: '
read login
if [ "$login" != "" ]; then printf 'password: '
while IFS= read -r -s -n1 char; do
  [[ -z $char ]] && { printf '\n'; break; } # ENTER pressed; output \n and break.
  if [[ $char == $'\x7f' ]]; then # backspace was pressed
      # Remove last char from output variable.
      [[ -n $password ]] && password=${password%?}
      # Erase '*' to the left.
      printf '\b \b' 
  else
    # Add typed char to output variable.
    password+=$char
    # Print '*' in its stead.
    printf '*'
  fi
done;fi
data=$(curl https://anitube.in.ua --data-raw "login=submit&login_name=$login&login_password=$password" -D - -o /dev/null > /tmp/cvd-gen 2>/dev/null; grep user_id /tmp/cvd-gen | cut -d' ' -f2 > /tmp/1; grep password /tmp/cvd-gen | cut -d' ' -f2 >> /tmp/1; paste -sd "" /tmp/1 | rev | cut -c2- | rev)
mkdir -p ~/.config/cvd
if [[ $torrserver != *none ]]; then echo torrserver=$torrserver > ~/.config/cvd/cvd.conf;fi
echo "libriamirror=https://anilibria.tv/public/torrent/download.php?id=
player=$player
torrent=$torrent
iregretnothing=$iregretnothing
res=$res
anitube.in.ua=$data
warn=$warn
over=$over
display=$display
norusso=$norusso
autoexpand=$autoexpand
#do not edit below
confverA=1
confverB=3
confverC=1" >> ~/.config/cvd/cvd.conf

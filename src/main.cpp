#ifdef unix
#include <pwd.h>
#endif
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
//сos? sin?
bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
unsigned short CountOccurrences(char* str, char* word)
{
    unsigned short count = 0;
    unsigned int strLen  = strlen(str);
    unsigned short wordLen = strlen(word);
    unsigned short j;

    for(int i=0; i <= strLen; i++)
    {
        for(j=0; j< wordLen; j++)
        {
            if(str[i + j] != word[j])
            {
                break;
            }
        }
        if(j == wordLen)
        {
            count++;
        }
    }
    return count;
}
std::string url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}
std::string to_utf8(uint32_t cp) //by Hfib
{
    std::string result;

    int count;
    if (cp <= 0x007F)
        count = 1;
    else if (cp <= 0x07FF)
        count = 2;
    else if (cp <= 0xFFFF)
        count = 3;
    else if (cp <= 0x10FFFF)
        count = 4;
    else
        return result;

    result.resize(count);

    if (count > 1)
    {
        for (int i = count-1; i > 0; --i)
        {
            result[i] = (char) (0x80 | (cp & 0x3F));
            cp >>= 6;
        }

        for (int i = 0; i < count; ++i)
            cp |= (1 << (7-i));
    }

    result[0] = (char)cp;
    return result;
}

void unescape(std::string &str) {
    std::string::size_type startIdx = 0;
    while(true) {
        startIdx = str.find("\\u", startIdx);
        if (startIdx == std::string::npos) break;

        std::string tmpStr = str.substr(startIdx + 2, 4);
        std::istringstream iss(tmpStr);

        uint32_t cp;
        if (iss >> std::hex >> cp)
        {
            std::string utf8 = to_utf8(cp);
            str.replace(startIdx, 6, utf8);
            startIdx += utf8.length();
        }
        else
            startIdx += 2;
    }
}
void AniLibriaById(std::string id,std::string qbittorrent,std::string addr,std::string mirror,std::string last,std::string mpv,std::string show_hidden){
       std::string resp = {httplib::Client ("http://api.anilibria.tv").Get("/v2/getTitle?show_hidden=" + show_hidden + "&filter=team.voice,type.length,playlist,torrents.list,player.host,player.series,player.playlist&id="+ id)->body};
           while (resp.find("\\\"") != std::string::npos)
       resp.replace(resp.find("\\\""), 2, "cvd");
           while (resp.find("\"alternative\":null") != std::string::npos)
         resp.replace(resp.find("\"alternative\":null"), 18, "\"alternative\":\"null\"");
           for(int also = 19; also < resp.length(); also++) {
               if(resp[also] == ']') {std::string sub = resp.substr(19,also-20);
                   while (sub.find("\",\"") != std::string::npos)
               sub.replace(sub.find("\",\""), 3, ", "); std::cout << "Озвучивали: " + sub << std::endl; break;}}
//            std::cout << resp.substr(resp.find("\"series\":")+18, 36); can hold 1000-9999 ser release
           if (addr != "" || qbittorrent != ""){std::cout << "Торренты:" << std::endl;
           std::string respt = resp.substr(0,resp.find("player"));
           unsigned short length = stod(respt.substr(respt.find("length")+8));
           for (unsigned short i=1;true;++i){
           std::string cur = resp.substr(resp.find("\"torrent_id\":"),resp.find(",\"hash\":")-resp.find("\"torrent_id\":"));
           std::string ser;
           short first = stod(cur.substr(cur.find("first")+7,4)); // -999 - 9999 серии
           short last = stod(cur.substr(cur.find("last")+6,4));
           unsigned short sers = last-first+1;
           std::cout << i << ") ";
           if (first != last){ser = cur.substr(cur.find("tring\":")+8, 11);
           std::cout << "Серии: " + ser.substr(0,ser.find("\"}")) << ", ";
           }
           ser = cur.substr(cur.find("y\":{\"string\":\"")+14); std::string typer = ser.substr(0, ser.find("\""));
           //           std::cout << ", " << ser.substr(0, ser.find("\"")) << ", ";
           std::cout << typer << ", ";
           std::string peers = ser.substr(ser.find("hers\":")+6);
           std::string peerss = "\033[32m" + peers.replace(peers.find(","),11,"\033[1;37m/\033[0;31m");
           std::cout << peerss.substr(0,peerss.find(",")) << "\033[0m, ";
           std::cout << peers.substr(peers.find("\"downloads\":")+12,peers.find(",\"total")-peers.find("\"downloads\":")-12) << " скачали";
           float size = std::stod(peers.substr(peers.find("e\":")+3));
           std::cout << ", " << round(size/10737418.24)/100 << "GB (" << round(size/length/sers/786432)/10 << "Mbit/sec)"; //7864320 - 2^20 * 60 / 8;
//           std::cout << ", " << round(std::stod(peers.substr(peers.find("e\":")+3))/10737418.24)/100 << "GB";
           unsigned long time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - std::stoull(peers.substr(peers.find("tamp\":")+6));
           if (time > 60) {if (time/31536000 != 0){std::cout << ", " << time/31536000 << " ";
               if (time/31536000 > 4 && time/31536000 < 21)std::cout << "лет";else
               if (time/31536000%10 == 1)std::cout << "год";else std::cout << "года";}
           if (time/86400%365 != 0){std::cout << ", " << time/86400%365;
               if (time/86400%365 > 4)std::cout << " дней";else
               if (time/86400%365%10 == 1)std::cout << " день";else std::cout << " дня";}
           if (time < 31536000){
           if (time/3600%24 != 0){std::cout << ", " << time/3600%24;
               if (time/3600%24 > 4)std::cout << " часов";else
               if (time/3600%24%10 == 1)std::cout << " час";else std::cout << " часа";}
           if (time/60%60 != 0 && time < 86400){std::cout << ", " << time/60%60;
               if (time/60%60 > 4)std::cout << " минут";else
               if (time/60%60 == 1)std::cout << " минуту";else std::cout << " минуты";}}
           std::cout << " назад";}
           else std::cout << ", только что залит!";
           std::cout << std::endl;
           if (cur.length()*2 < resp.find("player"))resp=resp.substr(cur.length()); else break;}
           std::string t;
           std::cin >> t;
           short want;
           if (t.length() > 1)want = stod(t.substr(1))-1;else want=-1;
           unsigned char t2=stod(t.substr(0,1));
           unsigned short i = 1;
           while (i<t2){respt=respt.substr(respt.find("id\"")+4);++i;}
           std::string idt=respt.substr(respt.find("torrent_id")+12);idt=idt.substr(0,idt.find(","));
           if (addr != "" && qbittorrent != "" && want == -1){
#ifdef unix
               std::cout<< "1) torrserver"<<std::endl<<"2) "<<qbittorrent<<std::endl; //"Введите 1 для открытия " << qbittorrent <<", введите 1XX для начала просмотра XX серии с помощью torrserver"<<std::endl;
#endif
#ifdef _WIN32
               std::cout << "1) torrserver" << std::endl << "2) torrent" << std::endl;
#endif
               std::string i;std::cin>>i;if(i=="2"){system((qbittorrent + " " + mirror + idt + last + "&").c_str());} else if (i.substr(0,1)=="1") {if (i.substr(0,1)==i) system((mpv + " \"http://" + addr + "/stream?link=" + mirror + idt + last + "&preload&m3u\"").c_str()); else system((mpv + " \"http://" + addr + "/stream?link=" + mirror + idt + last + "&preload&m3u\"" + " --playlist-start=" + std::to_string((stod(i.substr(1)) - 1))).c_str());}}else
           if (qbittorrent == "" || want != -1){if (mpv == "mplayer"){mpv="mplayer -playlist";if(addr.length()<30)mpv= "mplayer -prefer-ipv4 -playlist";}
#ifdef unix
               if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later" && want != -1)
#endif
#ifdef _WIN32
                   if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=" + std::string(getenv("APPDATA")) + "/cvd/mpv_watch_later" && want != -1)
#endif
           mpv = mpv + " --playlist-start="+std::to_string(want);
               system((mpv +" \"http://" + addr + "/stream?link=" + mirror + idt + last + "&preload&m3u\"").c_str());}
           else if (addr == "")system((qbittorrent + " " + mirror + idt + last + "&").c_str());}}
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
std::string monster(std::string original, unsigned char res){
std::string temp = original.substr(10);
httplib::Client player(original.substr(0,temp.find("/")+10));
player.set_follow_location(true);
std::string resp=player.Get(original.substr(temp.find("/")+10))->body;
resp = resp.substr(resp.find("file")+6);
resp = resp.substr(0,resp.find("\","));
if (CountOccurrences((char*)resp.c_str(),(char*)",[") < res) res=CountOccurrences((char*)resp.c_str(),(char*)",[");
for (char i=-1;i<res;i++)
resp=resp.substr(resp.find("]")+1);
return resp;
}
std::string parse(std::string original, unsigned char res,std::string player){
while (original.find("\\/") != std::string::npos)
original.replace(original.find("\\/"), 2, "/");
if (player == "М")return monster(original,res);
else return player + " is unsupported";
}
int main(int args,char** arg)
{
#ifdef _WIN32
system("@chcp 65001>NUL");
#endif
#ifdef unix
struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;
#endif
//     string line;
//       std::ifstream myfile ("/tmp/1");
//   if (myfile.is_open())
//   {
//     while ( getline (myfile,line) )
//     {
//       std::cout << line << '\n';
//     }
//     myfile.close();
//   }
       std::string limit = "50";
       std::string qbittorrent,addr,darklibria,dark,mpv,show_hidden;
       unsigned char monsterres = 2;
    // std::ifstream is RAII, i.e. no need to call close https://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c
    #ifdef unix
	std::ifstream cFile(homedir + std::string("/.config/cvd/cvd.conf"));
#endif
#ifdef _WIN32
 std::ifstream cFile(getenv("APPDATA") + std::string("/cvd/cvd.conf"));
#endif
if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line))
        {
#ifdef unix
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end()); //Program Files
#endif
            if (line[0] == '#' || line.empty()) continue;

            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            //Custom coding
            if (name == "limit") limit = value;
            else if (name == "player") if (value == "mpv")
                #ifdef unix
                mpv = "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later"
#endif
#ifdef _WIN32
                mpv = "mpv --write-filename-in-watch-later-config --watch-later-directory=" + std::string(getenv("APPDATA")) + "/cvd/mpv_watch_later"
#endif
                ;else mpv = value;
            else if (name == "darklike") dark = value;
            else if (name == "libriamirror") darklibria = value;
            else if (name == "torrserver") addr = value;
            else if (name == "torrent") qbittorrent = value;
            else if (name == "iregretnothing") show_hidden = value;
            else if (name == "monsterres") monsterres = stod(value);
            else if (name == "confverA") {if (stod(value) < 0){std::cout << "Конфиг устарел! Продолжать работу невозможно." << std::endl;return 1;}}
            else if (name == "confverB") {if (stod(value) < 2){std::cout << "Конфиг устарел! Новые функции недоступны." << std::endl;}}
            else if (name == "confverC") {if (stod(value) > 0){std::cout << "Версия программы устарела! Замените текущий конфиг на подходящий для этой версии или обновите программу. Продолжать работу невозможно." << std::endl; return 1;}}
        }
    }
    else std::cerr << "No user config file!" << std::endl;
std::string in,req;
    if(IsPathExist("/data/data/com.termux")){std::cout << "you're in termux. This is experemntal. The program WILL NOT fully work." << std::endl << "Continue?(y/n)";std::cin >> in; if(in == "n"){std::cout << "'No' detected, exiting.." << std::endl;return 0;}else{
system("/data/data/com.termux/files/usr/bin/termux-api-start >/dev/null&");}}
    if (args > 1)for(int i=0; i < strlen(arg[1]); i++)
                in=in+arg[1][i];else std::cin >> in;
    if (args > 2)limit=arg[2];
if (is_number(in)){if (stod(in) > 380 && stod(in) < 10000)AniLibriaById(in,qbittorrent,addr,darklibria,dark,mpv,show_hidden);}else
    try
{
if (in == "showlast")req="/v2/getChanges?filter=names,id&show_hidden=" + show_hidden + "&limit=" + limit;else
    if (in.find("anitube.in.ua") < 10){
        std::string resp = {httplib::Client ("https://anitube.in.ua").Get(in.substr(in.find("anitube.in.ua")+13))->body};
        unescape(resp);
        std::string vids = resp.substr(resp.find("RalodePlayer.init")+20);
        vids = vids.substr(0,vids.find("}]]")); //],[ разделяет плееры, }]] величает на конце
        std::string lists = vids.substr(0,vids.find("\"]"));
        unsigned short all = CountOccurrences((char*)lists.c_str(), (char*)"\",\"")+1;
        std::string playername[all];
        for (unsigned char i=0;i<all;i++){
            playername[i] = lists.substr(lists.find("ПЛЕЄР")+11,2);
            std::cout << std::to_string(i+1) << ") " << lists.substr(0,lists.find("\",\"")) << std::endl;
        lists=lists.substr(lists.find("\",\"")+3);}
        unsigned short need;
        while (true){
        std::cin >> need;
        if (playername[need-1] == "SI")std::cout << "Sibnet is unsuppored and not planned. Try another player" << std::endl;else break;}
        for (unsigned short i=0;i<need;i++)
            vids = vids.substr(vids.find("],[")+5);
        vids = vids.substr(0,vids.find("],["));
        all = CountOccurrences((char*)vids.substr(0,vids.find("],[")).c_str(), (char*)"}");
        std::cout << "Here's your m3u playlist!" << std::endl << "#EXTM3U" << std::endl;
        for (unsigned short i=0;i<all;i++){
std::cout << "#EXTINF:-1, " << vids.substr(vids.find(":")+2,vids.find("\",\"")-vids.find(":")-2) << std::endl << parse(vids.substr(vids.find("src=\\\"")+6,vids.find("frameborder")-vids.find("src=\\\"")-9),monsterres,playername[need-1]) << std::endl;
if (i + 1 != all)vids = vids.substr(vids.find("}")+9);}
std::cout << "#EXT-X-ENDLIST";
        return 0;
    //    echo \#EXTM3U>/tmp/sg.m3u;all=$(echo -e $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4) | grep -o { | wc -l);zero=0;while [ $all -gt $zero ]; do let zero=zero+1;echo -e \#EXTINF:-1, $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4 | cut -d{ -f$(($zero+1)) | cut -d\" -f4)>>/tmp/sg.m3u;curl $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4 | cut -d{ -f$(($zero+1)) | sed s/'.*src=\\\"'//g | cut -d\" -f1 | sed s/'\\\/'/'\/'/g | cut -d\\ -f1) -L | grep file | cut -d'
    //    ' -f1 | cut -c11- | cut -d\" -f1 | sed s/,/\\n/g | grep 720 | cut -d\] -f2>>/tmp/sg.m3u;done;echo \#EXT-X-ENDLIST >>/tmp/sg.m3u
    }
    else if (in.find("csst.online") < 10 || in.find("secvideo1.online") < 10){system((mpv + " " + monster(in,monsterres)).c_str());return 0;}
else req="/v2/searchTitles?filter=names,id&show_hidden=" + show_hidden + "&limit=" + limit + "&search=" + url_encode(in);
std::string resp = {httplib::Client ("http://api.anilibria.tv").Get(req)->body};
            while (resp.find("\\\"") != std::string::npos)
        resp.replace(resp.find("\\\""), 2, "cvd");
            while (resp.find("\"alternative\":null") != std::string::npos)
          resp.replace(resp.find("\"alternative\":null"), 18, "\"alternative\":\"null\"");
        unsigned short all = CountOccurrences((char*)resp.c_str(), (char*)"\"id\"");
        if (all > 1) {
    for (unsigned short i = 1; i != all+1; i++){std::cout << i << ") ";
        int comma = 0;
        int hit;
unsigned char ens = 0;
	for(int also = 0; also < resp.length(); also++) {
        if(resp[also] == '"') {
            comma++;
            if (comma == 5 + 16*(i-1))hit=also;
if (comma == 9 + 16*(i-1)){hit=also;ens=4;}
        }else if (comma == ens + 6 + 16*(i-1)) {
if (ens != 0) std::cout << " / ";
            std::string sub = resp.substr(hit+1, also-hit-2);
	    while (sub.find("cvd") != std::string::npos)//{
	    sub.replace(sub.find("cvd"), 3, "\"");//brokensub = 1;}
        std::cout << sub;
            if (ens != 0){std::cout << std::endl;break;}
        }
    }
};std::cout << "? ";std::cin >> in;
unsigned short i = std::stoul (in,nullptr,0);
int comma = 0;
int hit;
for(int also = 0; also < resp.length(); also++) {
    if(resp[also] == '"') {
        comma++;if (comma == 16*i){
            if (resp[also+5] == '}')AniLibriaById(resp.substr(also+2, 3),qbittorrent,addr,darklibria,dark,mpv,show_hidden);else
            AniLibriaById(resp.substr(also+2, 4),qbittorrent,addr,darklibria,dark,mpv,show_hidden);break;}}}
} else if (all == 0) {
    std::cout << "Нет результатов..." << std::endl;
} else {  // иначе
            int comma = 0;
    for(int also = 0; also < resp.length(); also++) {
        if(resp[also] == '"') {
            comma++;if (comma == 16){
                if (resp[also+5] == '}')AniLibriaById(resp.substr(also+2, 3),qbittorrent,addr,darklibria,dark,mpv,show_hidden);else
                AniLibriaById(resp.substr(also+2, 4),qbittorrent,addr,darklibria,dark,mpv,show_hidden);break;}}}
}
} catch (const std::exception& e)
    {
        std::cerr << "Failed. " << e.what() << std::endl;
        return 1;
    }
return 0;
}
//curl 'https://www.anilibria.tv/public/login.php' --data-raw 'mail=van.a.gribanov@gmail.com&passwd=****&fa2code=' | jq .sessionId
//curl 'https://toloka.to/tracker.php?nm=chainsaw' -H 'Cookie: toloka_sid=***' | sed ':a;N;$!ba;s/\n/cvdnewlinecvd/g' | sed 's/.*<\/th>'//g | sed s/cvdnewlinecvd/\\n/g
// cookie=$(curl 'https://toloka.to/login.php' --data-raw 'username=van.a.gribanov@gmail.com&password=***&autologin=on&login=Вхід' -c - | grep toloka_sid | cut -c47-)

#include <iostream>
#include "HTTPRequest.hpp"
#include <sys/stat.h>
#include <fstream>
#include <pwd.h>
#include <sstream>
#include <iomanip>
#include <cmath>
//An unnamed cvd cxx build. Hello, github!
bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
unsigned short CountOccurrences(char* str, char* word)
{
    int count = 0;
    int strLen  = strlen(str);
    int wordLen = strlen(word);
    int j;

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
void AniLibriaById(std::string id,std::string qbittorrent,std::string addr,std::string mirror,std::string last,std::string mpv){
http::Request request{"http://api.anilibria.tv/v2/getTitle?show_hidden=1&filter=team.voice,type.length,playlist,torrents.list,player.host,player.series,player.playlist&id="+ id};
const auto response = request.send("GET");
       std::string resp = {response.body.begin(), response.body.end()};
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
           if (cur.length()*2 < resp.find("player"))resp=resp.substr(cur.length()); else break;
}
           std::string t;
           std::cin >> t;
           short want;
           if (t.length() > 1)want = stod(t.substr(1))-1;else want=-1;
           unsigned char t2=stod(t.substr(0,1));
           unsigned short i = 1;
           while (i<t2){respt=respt.substr(respt.find("id\"")+4);++i;}
           std::string idt=respt.substr(respt.find("torrent_id")+12);idt=idt.substr(0,idt.find(","));
           if (addr != "" && qbittorrent != "" && want == -1){std::cout<< "1) torrserver"<<std::endl<<"2) "<<qbittorrent<<std::endl; //"Введите 1 для открытия " << qbittorrent <<", введите 1XX для начала просмотра XX серии с помощью torrserver"<<std::endl;
unsigned short i;std::cin>>i;if(i==2){system((qbittorrent + " " + mirror + idt +last + "&").c_str());} else if (i==1)system((mpv +" 'http://" + addr + "/stream?link=" + mirror + idt +last + "&preload&m3u'").c_str());}else
           if (qbittorrent == "" || want != -1){if (mpv == "mplayer"){mpv="mplayer -playlist";if(addr.length()<30)mpv= "mplayer -prefer-ipv4 -playlist";}
           if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=~/config/cvd/mpv_watch_later" && want != -1){mpv = mpv + " --playlist-start="+std::to_string(want);}
               system((mpv +" 'http://" + addr + "/stream?link=" + mirror + idt +last + "&preload&m3u'").c_str());}
           else if (addr == "")system((qbittorrent + " " + mirror + idt +last + "&").c_str());}}
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
int main(int args,char** arg)
{
struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;
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
       unsigned char limit = 50;
       std::string qbittorrent;
       std::string addr;
       std::string darklibria;
       std::string dark;
       std::string mpv;
    // std::ifstream is RAII, i.e. no need to call close https://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c
    std::ifstream cFile(homedir + std::string("/.config/cvd/cvd.conf"));
    if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end());
            if (line[0] == '#' || line.empty()) continue;

            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            //Custom coding
            if (name == "limit") limit = stod(value);
            else if (name == "player") mpv = value;
            else if (name == "darklike") dark = value;
            else if (name == "libriamirror") darklibria = value;
            else if (name == "torrserver") addr = value;
            else if (name == "torrent") qbittorrent = value;
        }
    }
    else std::cerr << "No user config file!" << std::endl;
if (mpv == "mpv")mpv="mpv --write-filename-in-watch-later-config --watch-later-directory=~/config/cvd/mpv_watch_later";
std::string in,req;
    if(IsPathExist("/data/data/com.termux")){std::cout << "you're in termux. This is experemntal. The program WILL NOT fully work." << std::endl << "Continue?(y/n)";std::cin >> in; if(in == "n"){std::cout << "'No' detected, exiting.." << std::endl;return 0;}else{
system("/data/data/com.termux/files/usr/bin/termux-api-start >/dev/null&");}}
    std::cout << "AniLibria search." << std::endl;
    if (args > 1)for(int i=0; i < strlen(arg[1]); i++)
                in=in+arg[1][i];else std::cin >> in;
    if (args > 2)limit=std::stod(arg[2]);
if (is_number(in)){if (stod(in) > 380 && stod(in) < 10000)AniLibriaById(in,qbittorrent,addr,darklibria,dark,mpv);}else
    try
{
if (in == "showlast")req="http://api.anilibria.tv/v2/getChanges?limit=" + std::to_string(limit) + "&filter=names,id&show_hidden=1"; else
req="http://api.anilibria.tv/v2/searchTitles?limit=" + std::to_string(limit) + "&filter=names,id&show_hidden=1&search=" + url_encode(in);
http::Request request{req};
const auto response = request.send("GET");
 std::cout << "got ?" << std::endl;
        std::string resp = {response.body.begin(), response.body.end()};
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
            if (resp[also+5] == '}')AniLibriaById(resp.substr(also+2, 3),qbittorrent,addr,darklibria,dark,mpv);else
            AniLibriaById(resp.substr(also+2, 4),qbittorrent,addr,darklibria,dark,mpv);break;}}}
} else if (all == 0) {
    std::cout << "Нет результатов..." << std::endl;
} else {  // иначе
            int comma = 0;
    for(int also = 0; also < resp.length(); also++) {
        if(resp[also] == '"') {
            comma++;if (comma == 16){
                if (resp[also+5] == '}')AniLibriaById(resp.substr(also+2, 3),qbittorrent,addr,darklibria,dark,mpv);else
                AniLibriaById(resp.substr(also+2, 4),qbittorrent,addr,darklibria,dark,mpv);break;}}}
}
    //std::cout << resp << std::endl;
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
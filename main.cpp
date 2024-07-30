#ifdef unix
#include <pwd.h>
#endif
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <filesystem>
#include <cstdlib>
//#include <future>
// But the cat murmur


bool IsPathExist(const std::string &s) {
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

unsigned short CountOccurrences(char *str, char *word) {
    unsigned short count = 0;
    unsigned int strLen  = strlen(str);
    unsigned short wordLen = strlen(word);
    unsigned short j;

    for(unsigned int i=0; i <= strLen; ++i) {
        for(j=0; j< wordLen; j++) {
            if(str[i + j] != word[j]) {
                break;
            }
        }
        if(j == wordLen) {
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

std::string to_utf8(uint32_t cp) { //by Hfib

    std::string result;

    int count;
    if (cp <= 0x007F) {
        count = 1;
    } else if (cp <= 0x07FF) {
        count = 2;
    } else if (cp <= 0xFFFF) {
        count = 3;
    } else if (cp <= 0x10FFFF) {
        count = 4;
    } else {
        return result;
    }

    result.resize(count);

    if (count > 1) {
        for (int i = count-1; i > 0; --i) {
            result[i] = (char) (0x80 | (cp & 0x3F));
            cp >>= 6;
        }

        for (int i = 0; i < count; ++i) {
            cp |= (1 << (7-i));
        }
    }

    result[0] = (char)cp;
    return result;
}

void unescape(std::string &str) {
    std::string::size_type startIdx = 0;
    while(true) {
        startIdx = str.find("\\u", startIdx);
        if (startIdx == std::string::npos) {
            break;
        }

        std::string tmpStr = str.substr(startIdx + 2, 4);
        std::istringstream iss(tmpStr);

        uint32_t cp;
        if (iss >> std::hex >> cp) {
            std::string utf8 = to_utf8(cp);
            str.replace(startIdx, 6, utf8);
            startIdx += utf8.length();
        } else {
            startIdx += 2;
        }
    }
}

void torrent(std::string link,std::string torrserver,std::string qbittorrent,std::string mpv,short want) {
    if (torrserver != "" && qbittorrent != "" && want == -1){
#ifdef unix
        std::cout<< "1) torrserver"<<std::endl<<"2) "<<qbittorrent<<std::endl; //"Введите 1 для открытия " << qbittorrent <<", введите 1XX для начала просмотра XX серии с помощью torrserver"<<std::endl;
#endif
#ifdef _WIN32
        std::cout << "1) torrserver" << std::endl << "2) torrent" << std::endl;
#endif
        std::string i;std::cin>>i;
        if(i=="2") {
            system((qbittorrent + " '" + link + "\'>/dev/null&").c_str());
        } else if (i.substr(0,1)=="1") {
            if (i.substr(0,1)==i) {
                system((mpv + " \"http://" + torrserver + "/stream?link=" + url_encode(link) + "&preload&m3u\"").c_str());
            } else {
                system((mpv + " \"http://" + torrserver + "/stream?link=" + url_encode(link) + "&preload&m3u\"" + " --playlist-start=" + std::to_string(short(stod(i.substr(1)) - 1))).c_str());
            }
        }
    } else if (qbittorrent == "" || want != -1) {
        if (mpv == "mplayer") {
            mpv="mplayer -playlist";
            if(torrserver.length()<30) {
                mpv= "mplayer -prefer-ipv4 -playlist";
            }
        }
#ifdef unix
        if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later" && want != -1) {
#endif
#ifdef _WIN32
            if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=" + std::string(getenv("APPDATA")) + "/cvd/mpv_watch_later" && want != -1) {
#endif
            mpv = mpv + " --playlist-start="+std::to_string(want);
        }
        system((mpv +" \"http://" + torrserver + "/stream?link=" + url_encode(link) + "&preload&m3u\"").c_str());
    } else if (torrserver == "") {
        system((qbittorrent + " '" + link + "\'&").c_str());
    }
}

void AniLibriaById(std::string id,std::string qbittorrent,std::string addr,std::string mirror,std::string last,std::string mpv,std::string show_hidden,std::string lc,
                   #ifdef _WIN32
                   std::wstring sounded,std::wstring sec,std::wstring skachali,std::wstring serii
                   #else
                   std::string sounded,std::string sec,std::string skachali,std::string serii
                   #endif
                   ,bool warn){
    std::string torrents,lnull,let,god,goda,dnei,den,dnya,chasov,chasa,minut,minutu,chas,minuti,nazad,justnow,notorrents;
    if (lc == "uk" || lc == "Ukr" || lc == "ru" || lc == "Rus") {
        torrents = "Торенти";
        lnull = "Тривалість відео невідома! Бітрейт рахується від припущення, що одна серія триває 24 хвилини";
        let = "років";god="рік";goda="роки";dnei="днів";den="день";dnya="дні";chasov="годин";chasa="години";minut="хвилин";minutu="хвилину";chas="годину";minuti="хвилини";nazad="тому";
        justnow="щойно залит";
        notorrents="Нема торентів...";
    } else {
        //if (lc == "ru" || lc == "Rus"){
        //torrents = "Торренты";
        //lnull = "Длительность видео неизвестна! Битрейт считается от предположения, что одна серия длиться 24 минуты";
        //let = "лет";god="год";goda="года";dnei="дней";den="день";dnya="дня";chasov="часов";chasa="часа";minut="минут";minutu="минуту";chas="час";minuti="минуты";nazad="назад";
        //serii="Серии";skachali="скачали";justnow="только что залит";}else{
        torrents = "Torrents";
        lnull = "Video length in unknown. Bitraite is considered to be the assumption that one series lasts 24 minutes";
        let = "years";god="year";goda = "years";dnei="days";den="day";dnya="days";chasov="hours";chasa="hours";minut="minutes";minutu="minute";chas="hour";minuti="minutes";nazad="ago";
        justnow="just uploaded";
        notorrents="No torrents...";
    }
    std::string resp = {httplib::Client ("http://api.anilibria.tv").Get("/v2/getTitle?show_hidden=" + show_hidden + "&filter=team.voice,type.length,playlist,torrents.list,player.host,player.series,player.playlist&id=" + id)->body};
    while (resp.find("\\\"") != std::string::npos) {
        resp.replace(resp.find("\\\""), 2, "cvd");
    }
    while (resp.find("\"alternative\":null") != std::string::npos) {
        resp.replace(resp.find("\"alternative\":null"), 18, "\"alternative\":\"null\"");
    }
    for(int also = 19; also < resp.length(); also++) {
        if(resp[also] == ']') {
            std::string sub = resp.substr(19,also-20);
            if (std::to_string(sub[0]) != "125") {
                while (sub.find("\",\"") != std::string::npos) {
                    sub.replace(sub.find("\",\""), 3, ", ");
                }
#ifdef _WIN32
                std::wcout << sounded;std::cout << ": " << sub << std::endl;
#else
                std::cout << sounded << ": " << sub << std::endl;
#endif
                break;
            }
        }
    }
//  std::cout << resp.substr(resp.find("\"series\":")+18, 36); can hold 1000-9999 ser release
    unsigned short lasti=0;
    bool found;
    if ((addr != "" || qbittorrent != "") && (resp.find("\"torrent_id\":") != std::string::npos)) {
        std::cout << torrents << ":" << std::endl;
        std::string respt = resp.substr(0,resp.find("player"));
//      std::cout << "length=" << length;
//      if (resp.find(resp.find("\"torrent_id\":")) != std::string::npos){
        unsigned short length;
        if (respt.substr(respt.find("length")+8,4) != "null") {
            length = stod(respt.substr(respt.find("length")+8));
        } else {
            if (warn) {
                std::cout << lnull << std::endl;length = 24;
            }
        }
        found=1;
        for (unsigned short i=1;true;++i) {
            std::string cur = resp.substr(resp.find("\"torrent_id\":"),resp.find(",\"hash\":")-resp.find("\"torrent_id\":"));
            std::string ser;
            short first = stod(cur.substr(cur.find("first")+7,4)); // -999 - 9999 серии
            short last = stod(cur.substr(cur.find("last")+6,4));
            unsigned short sers = last-first+1;
            std::cout << i << ") ";
            if (first != last) {
                ser = cur.substr(cur.find("tring\":")+8, 11);
                std::cout << serii << ": " + ser.substr(0,ser.find("\"}")) << ", ";
            }
            ser = cur.substr(cur.find("y\":{\"string\":\"")+14);
            std::string typer = ser.substr(0, ser.find("\""));
            //std::cout << ", " << ser.substr(0, ser.find("\"")) << ", ";
            std::cout << typer << ", ";
            std::string peers = ser.substr(ser.find("hers\":")+6);
            std::string peerss = "\033[32m" + peers.replace(peers.find(","),11,"\033[1;37m/\033[0;31m");
            std::cout << peerss.substr(0,peerss.find(",")) << "\033[0m, ";
#ifdef _WIN32
            std::cout << peers.substr(peers.find("\"downloads\":")+12,peers.find(",\"total")-peers.find("\"downloads\":")-12) << " ";
            std::wcout << skachali;
#else
            std::cout << peers.substr(peers.find("\"downloads\":")+12,peers.find(",\"total")-peers.find("\"downloads\":")-12) << " " << skachali;
#endif
            float size = std::stod(peers.substr(peers.find("e\":")+3));
#ifdef _WIN32
            std::cout << ", " << round(size/10737418.24)/100 << "GB (" << round(size/length/sers/786432)/10 << "Mbit/";std::wcout << sec;std::cout << ")";
#else
            std::cout << ", " << round(size/10737418.24)/100 << "GB (" << round(size/length/sers/786432)/10 << "Mbit/" << sec << ")"; //7864320 - 2^20 * 60 / 8;
#endif
//          std::cout << ", " << round(std::stod(peers.substr(peers.find("e\":")+3))/10737418.24)/100 << "GB";
            unsigned long time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - std::stoull(peers.substr(peers.find("tamp\":")+6));
            if (time > 60) {
                if (time/31536000 != 0) {
                    std::cout << ", " << time/31536000 << " ";
                    if (time/31536000 > 4 && time/31536000 < 21) {
                        std::cout << let;
                    } else {
                        if (time/31536000%10 == 1) {
                            std::cout << god;
                        } else {
                            std::cout << goda;
                        }
                    }
                }
                if (time/86400%365 != 0) {
                    std::cout << ", " << time/86400%365;
                    if (time/86400%365%10 > 4) {
                        std::cout << " " << dnei;
                    } else {//fix it
                        if (time/86400%365%10 == 1) {
                            std::cout << " " << den;
                        } else {
                            std::cout << " " << dnya;
                        }
                    }
                }
                if (time < 31536000) {
                    if (time/3600%24 != 0){
                        std::cout << ", " << time/3600%24;
                        if (time/3600%24 > 4) {
                            std::cout << " " << chasov;
                        } else {
                            if (time/3600%24%10 == 1) {
                                std::cout << " " << chas;
                            } else {
                                std::cout << " " << chasa;
                            }
                        }
                    }
                    if (time/60%60 != 0 && time < 86400) {
                        std::cout << ", " << time/60%60;
                        if (time/60%60 > 4) {
                            std::cout << " " << minut;
                        } else {
                            if (time/60%60 == 1) {
                                std::cout << " " << minutu;
                            } else {
                                std::cout << " " << minuti;
                            }
                        }
                    }
                }
                std::cout << " " << nazad;
            } else {
                std::cout << ", " << justnow << "!";
            }
            std::cout << std::endl;
            if (cur.length()*2 < resp.find("player")) {
                resp=resp.substr(cur.length());
            } else {
                lasti=i;break;  //}else std::cout << notorrents << std::endl;
            }
        }
        //std::cout << lasti;
        if (found) {
            std::string t;
            //std::cout << resp;
            std::cin >> t;
            short want;
            if (t.length() > 1) {
                want = stod(t.substr(1))-1;
            } else {
                want=-1;
            }
            unsigned char t2=stod(t.substr(0,1));
            unsigned short i = 1;
            while (i<t2) {
                respt=respt.substr(respt.find("id\"")+4);++i;
            }
            std::string idt=respt.substr(respt.find("torrent_id")+12);idt=idt.substr(0,idt.find(","));
            idt = mirror + idt + last;
            torrent(idt,addr,qbittorrent,mpv,want);
        }
    }
}

bool is_number(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
        return !std::isdigit(c);
    }) == s.end();
}

std::string preuniversal(std::string original) {
    return original.substr(0,original.find("/",11));
}

std::string prepreuniversal(std::string resp) {
    return resp.substr(resp.find("file")+6);
}

std::string universal(std::string resp) {
    resp = prepreuniversal(resp);
    return resp.substr(0,resp.find(",")-1);
}

std::string veryuniversal(std::string original) {
    std::string host = preuniversal(original);
    return universal({httplib::Client (host).Get(original.substr(host.length()))->body});
//  std::string hoststr = preuniversal(original);
//  httplib::Client host(hoststr);
//  host.set_follow_location(true);
//  return universal({host.Get(original.substr(hoststr.length()))->body});
}

std::string monstrolike(std::string resp,signed char res) {
    if (CountOccurrences((char*)resp.c_str(),(char*)",[") < res) {
        res=CountOccurrences((char*)resp.c_str(),(char*)",[");
    }
    for (signed char i=-1;i<res;i++) {// char is wrong for armhf gcc
        resp=resp.substr(resp.find("]")+1);
    }
    return resp.substr(0,resp.find(","));
}

std::string monster(std::string original, unsigned char res,
#ifdef _WIN32
    std::wstring
#else
    std::string
#endif
alert) {
    std::string resp;
    try {
        resp = {httplib::Client ("https://secvideo1.online").Get(original.substr(original.find("/",15)))->body};
    } catch(...) {
        std::string temp = original.substr(10);
        httplib::Client player(original.substr(0,temp.find("/")+10));
        player.set_follow_location(true);
        resp=player.Get(original.substr(temp.find("/")+10))->body;
#ifdef _WIN32
        std::wcout
#else
    std::cout
#endif
        << std::endl << alert << 2 << std::endl;
    }
    resp = prepreuniversal(resp);
    return monstrolike(resp.substr(0,resp.find("\",")),res);
}

std::string peer(std::string original) {
    std::string resp = {httplib::Client ("https://www.mp4upload.com").Get(original.substr(25))->body};
    resp = resp.substr(resp.find("embed")+6,resp.find("|src")-resp.find("embed")-6);
    std::string videos = resp.substr(resp.find("video|")+6);
    return "https://" + resp.substr(0,resp.find("|")) + ".mp4upload.com:" + videos.substr(videos.find("|")+1) + "/d/" + videos.substr(0,videos.find("|")) + "/video.mp4";
}

std::string udrop(std::string original) {
    std::string resp = {httplib::Client ("https://www.udrop.com").Get(original.substr(21))->body};
    resp = resp.substr(resp.find("mp4HD")+8);
    return resp.substr(0,resp.find("\""));
}

std::string realpeer(std::string original, unsigned short res) {
    std::string host=preuniversal(original);
    std::string resp = {httplib::Client (host).Get("/api/v1/videos"+original.substr(host.length()+13))->body};
    resp = resp.substr(resp.find("resolution")+18);
    while (stod(resp) > res) {
        short pos = resp.find("resolution");
        if (pos != -1) {
            resp = resp.substr(pos+18);
        } else {
            break;
        }
    }
    resp = resp.substr(resp.find("fileDownloadUrl")+18);return resp.substr(0,resp.find(",")-1);
}

std::string gg(std::string original) {
    original = original.substr(original.find("/d/")+3);
    return "http://docs.google.com/uc?export=download&confirm=t&id=" + original.substr(0,original.find("/")); //mpv 'https://drive.usercontent.google.com/download?confirm=t&id=1r3yteYO866tFO1hMvgE9ykf48-lur10j'
}

std::string ufdub(std::string str) {
    std::string resp = {httplib::Client ("https://video.ufdub.com").Get("/V2/VP.php?ID="+str.substr(str.find("=")+1))->body};
    str = resp.substr(resp.find("[")+34);
    str = str.substr(0,str.find("]]"));
    while (str.find("['") != std::string::npos) {
        str.replace(str.find("['"), 2, "#EXTINF:-1, ");
        str.replace(str.find("','"), 9, "\n");
        str.erase(str.find("'"),3);
    }
    return str;
}

std::string moon(std::string original,unsigned char res) {
    std::string resp = {httplib::Client ("https://moonanime.art").Get(original.substr(21))->body};//замінити на 0 при проблемах
    if (resp.substr(resp.find("file")+6,1)=="[") {
        resp=monstrolike(resp.substr(resp.find("\",")),res-2);
        if (resp[resp.length()-1]=='"') {
            return resp.substr(0,resp.length()-1);
        } else {
            return resp;
        }
    } else {
        return universal(resp);
    }
}

std::string un(std::string in,std::string un) {
    while (in.find(un) != std::string::npos) {
        in.erase(in.find(un), 1);
    }
    return in;
}

std::string parse(std::string original, unsigned short res, std::string &mpvargs, std::string mpv, std::string lc,
#ifdef _WIN32
std::wstring
#else
std::string
#endif
alert) {
    original=un(original,"\\/");
    if (original.find("\"") < 100) {
        original=original.substr(0,original.find("\"")-1); // no idea
    }
    std::string player = original.substr(8,6);
    if ((player == "csst.o") || (player == "secvid") || (player == "onvi.c") || (player == "monstr")) { //(player == "М" || player == "CS")
        return monster(original,res-1,alert);
    } else if ((player == "ashdi.") || (player == "tortug")) { //(player == "AS")
        return veryuniversal(original);
    } else if (player == "moonan") { //(player == "TR" || player == "TO")
        return moon(original+"/",res);
    } else if (player == "www.mp") { //(player == "PE" || player == "MP")
        if (mpv == "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later") {
            mpvargs="--http-header-fields='Referer'";
        } else {
            if (lc == "uk" || lc == "Ukr" || lc == "ru" || lc == "Rus") {
                std::cerr << "У разі, якщо щось не працює використовуйте mpv. Якщо ви бажаєте, щоб я зробив можливим перегляд з вашого улюбленого плеєра - напишіть мені" << std::endl;
            } else { //if (lc == "ru" || lc == "Rus") std::cerr << "Используйте mpv, если что-то не работает" << std::endl;else
                std::cerr << "In case of any problems use mpv. If you want fix for your player - let me know" << std::endl;
            }
        }
        return peer(original);
    } else if (player == "www.ud") { //(player == "UD")
        return udrop(original);
    } else if ((player == "peertu") || (player == "videos") || (player == "freedi")) {
        return realpeer(original,res);
    } else if (player == "drive.") {
        return gg(original);
    } else if (player == "video.") {
        return ufdub(original);
    } else {
        return original; //"Player is unsupported. Please Report " + player +
    }
}

// std::string normalize(std::string req){
//     httplib::Headers headers = {{"X-MAL-Client-ID", "6114d00ca681b7701d1e15fe11a4987e"}};
//     std::string resp {httplib::Client ("https://api.myanimelist.net").Get("/v2/anime?limit=1&fields=alternative_titles&q=" + url_encode(req), headers)->body};
//     resp = resp.substr(resp.find("]")+8);
//     resp = resp.substr(0,resp.find(",\"")-1);
//     httplib::Headers headers = {{"X-MAL-Client-ID", "6114d00ca681b7701d1e15fe11a4987e"}};
//     std::string resp {httplib::Client ("https://api.myanimelist.net").Get("/v2/anime?limit=1&q=" + url_encode(req), headers)->body};
//     resp = resp.substr(resp.find("title")+8);
//     resp = resp.substr(0,resp.find(",\"")-1);
//     unescape(resp);
//     return resp;
// }

void spaces(unsigned short all) {
    std::string spaces;
    for (short f=-1;f<all; ++f) {
        //std::cout << " ";
        //system("printf ' '");
        spaces=spaces+" ";
    //std::cout << "]\r ";
    }
#ifdef _WIN32
    std::cout << spaces << "]\r[";
#else
    system(("printf '" + spaces + "]\r['&").c_str());
#endif
}

std::vector <std::string> playerD(const std::string resp, unsigned short &last,std::string &vids) {
    vids = resp.substr(resp.find("RalodePlayer.init")+20);
    vids = vids.substr(0,vids.find("}]]")); //],[ разделяет плееры, }]] величает на конце
    std::string lists = vids.substr(0,vids.find("\"]"));
    unsigned short all = CountOccurrences((char*)lists.c_str(), (char*)"\",\"")+1; //char semi tested
    last = all;
    std::vector <std::string> playername(all);
    for (unsigned char i=0;i<all;i++) {
        playername[i] = lists.substr(lists.find("ПЛЕЄР")+11,2);
        //if (all>1) а ще тут була загадкова "П", яку, проте, використовують для слова "ПРОТО", що csst
        if (playername[i] == "SI" || playername[i] == "MY" || playername[i] == "PR" || playername[i] == "SO" || playername[i] == "С" || playername[i] == "SB") {
            std::cout <<
#ifdef _WIN32
    "UNSUPPORTED";
#else
    "\e[9m\e[1;30m";
#endif
        }
        std::cout << std::to_string(i+1) << ") " << lists.substr(0,lists.find("\",\"")) << std::endl << "\e[0m";
        lists=lists.substr(lists.find("\",\"")+3);
    }
    return playername;
}

std::string chooseP(const unsigned char res, std::string mpv, unsigned short all, std::string vids, std::string orig, const std::vector<std::string> playername,const std::string lc,const unsigned short need,bool over,
#ifdef _WIN32
                    std::wstring
#else
    std::string
#endif
    testfunc) {
    std::string mpvargs;
    //std::string sibnet;
    //if (lc == "uk" || lc == "Ukr" || lc == "ru" || lc == "Rus")
    //sibnet = "Плеєр не підтримуєтся. Оберіть інший плеєр. Якщо вам дійсно потрібна підтримка цього пдлеєру - дайте мені знати";else
    ////if (lc == "ru" || lc == "Rus"){
    ////sibnet = "Плеер не поддерживается и не собирается. Выберите другой плеер";}else
    //sibnet = "Player is unsuppored and not planned. Try another player. Let me know if you're really need this";
    if (all>1) {
        //while (true){
        //std::cin >> need;
        //if (playername[need-1] == "SI" || playername[need-1] == "MY" || playername[need-1] == "PR" || playername[need-1] == "П")std::cout << sibnet << std::endl;else break;}}else need = 1;
        for (unsigned short i=0;i<need;i++) {
            vids = vids.substr(vids.find("],[")+5);
        }
        vids = vids.substr(0,vids.find("],["));
        all = CountOccurrences((char*)vids.substr(0,vids.find("],[")).c_str(), (char*)"}");
        //if (all>0){
        std::ofstream m3u;
        //std::string orig = resp.substr(resp.find("https://plus.google.com/share?url=")+34);orig=orig.substr(0,orig.find("%20"));
        if (over) {
            m3u.open (std::filesystem::temp_directory_path().string()+"/cvd.m3u");
        } else {
            m3u.open (std::filesystem::temp_directory_path().string()+"/"+orig+".m3u");
        }
        //std::cout << "opened " << std::filesystem::temp_directory_path().string() << "/" << orig << ".m3u" << std::endl;
        m3u << "#EXTM3U" << std::endl;
        //std::cout << "[";
        spaces(all);
        for (unsigned short i=0;i<all;i++) {
            m3u << "#EXTINF:-1, " << un(vids.substr(vids.find(":")+2,vids.find("\",\"")-vids.find(":")-2),"\\/") << std::endl << parse(vids.substr(vids.find("src=\\\"")+6,vids.find("frameborder")-vids.find("src=\\\"")-9),res,mpvargs,mpv,lc,testfunc) << std::endl; //playername[need-1]
            std::cout << '|' << std::unitbuf;
            if (i + 1 != all) {
                vids = vids.substr(vids.find("}")+9);
            }
        }
        m3u << "#EXT-X-ENDLIST";
        m3u.close();
        std::cout << std::endl;
        if (over) {
            return mpv + " '" + std::filesystem::temp_directory_path().string()+"/cvd.m3u' " + mpvargs;
        } else {
            return mpv + " '" + std::filesystem::temp_directory_path().string()+"/"+orig+".m3u' " + mpvargs;
        }
    } else {
        if (mpv ==
#ifdef unix
        "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later"
#endif
#ifdef _WIN32
        "mpv --write-filename-in-watch-later-config --watch-later-directory=" + std::string(getenv("APPDATA")) + "/cvd/mpv_watch_later"
#endif
        )mpv=mpv + " --force-media-title='" + orig +"'";
        return mpv + " '" + parse(vids.substr(vids.find("src=\\\"")+6,vids.find("frameborder")-vids.find("src=\\\"")-9),res,mpvargs,mpv,lc,testfunc) + "' " + mpvargs; //playername[0]
        //    echo \#EXTM3U>/tmp/sg.m3u;all=$(echo -e $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4) | grep -o { | wc -l);zero=0;while [ $all -gt $zero ]; do let zero=zero+1;echo -e \#EXTINF:-1, $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4 | cut -d{ -f$(($zero+1)) | cut -d\" -f4)>>/tmp/sg.m3u;curl $(curl https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html | grep RalodePlayer.init | cut -d[ -f4 | cut -d{ -f$(($zero+1)) | sed s/'.*src=\\\"'//g | cut -d\" -f1 | sed s/'\\\/'/'\/'/g | cut -d\\ -f1) -L | grep file | cut -d'
        //    ' -f1 | cut -c11- | cut -d\" -f1 | sed s/,/\\n/g | grep 720 | cut -d\] -f2>>/tmp/sg.m3u;done;echo \#EXT-X-ENDLIST >>/tmp/sg.m3u}
    }
}

unsigned short playerajaxD(std::string who,std::vector<std::string> &links) {
    unsigned short i=0;
    //unsigned char allm=CountOccurrences((char*)who.c_str(),(char*)"data-id");
    while (who.find("data-id") != std::string::npos) {
        ++i;
        links.push_back(who.substr(0,who.find("<")));
        std::cout << i << ") " << links[i-1]<<std::endl;
        who = who.substr(who.find("\">")+2);
    }
    ++i;
    links.push_back(who.substr(0,who.find("<")));
    //if (allm !=0)
        std::cout << i << ") " << links[i-1]<<std::endl;
    return i;
}

void simplevoiced(std::string resp,bool nl,std::string sounded) {
    std::string voiced;
    if (resp.find("h/v") == std::string::npos) {
      voiced = resp.substr(resp.find("https://anitube.in.ua/xfsearch/translation/"));
      sounded="Переклад";
    } else {
    voiced = resp.substr(resp.find("https://anitube.in.ua/xfsearch/voiced/"));
    }
#ifdef _WIN32
    std::wcout << sounded;std::cout << ": ";
#else
    if (nl) {
        std::cout << "\e[1m";
    } else {
        std::cout << "\e[92m";
    }
    std::cout << sounded << "\e[0m: ";
#endif
    voiced = voiced.substr(0,voiced.find("<div"))+"href  ";
    while (voiced.find("&#039;") != std::string::npos) {
        voiced.replace(voiced.find("&#039;"), 6, "'");
    }
    while (voiced.find("&quot;") != std::string::npos) {
        voiced.replace(voiced.find("&quot;"), 6,"\"");
    }
    for (unsigned short i; voiced.length()>40;++i) {
        std::cout << voiced.substr(voiced.find(">")+1,voiced.find("<")-voiced.find(">")-1);
        voiced = voiced.substr(voiced.find("href")+6);
        if (voiced.length()>40) {
            std::cout << ", ";
        }
    }
    if (nl) {
        std::cout << std::endl;
    }
}

std::string getsers(std::string resp) {
    std::string sers = resp.substr(resp.find("ій:"));//+11);
    sers=sers.substr(sers.find(">")+1);
    if (sers.substr(0,1) == " ") {
        sers.erase(0,1);
    }
    // while (sers.substr(sers.size()-1,1) == " ") {
    // sers.erase(sers.size()-1,1);
    // }
    while (sers.find("\n") != std::string::npos) {
        sers.replace(sers.find("\n"),1," ");
    }
    sers=sers.substr(0,sers.find("<"));sers=un(sers,"  ");
    return sers;
}

void anitube(std::string in, std::string mpv,std::string lc,unsigned short res,httplib::Headers headers,std::string torrserver,std::string qbittorrent,bool over,bool display,
#ifdef _WIN32
std::wstring skachali,std::wstring sec,std::wstring sounded,std::wstring testfunc,std::wstring serii){
#else
std::string skachali,std::string sec,std::string sounded,std::string testfunc,std::string serii){
#endif
#ifdef _WIN32
    std::wstring np,nt;
#else
    std::string np,nt;
#endif
    if (lc == "uk" || lc == "Ukr" || lc == "ru" || lc == "Rus") {
#ifdef _WIN32
        np = L"Не знайдено плеєрів!";
        nt = L"Не знайдено торентів!";
    } else {
        np = L"No players was found";
        nt = L"No torrents was found";
    }
#else
        np = "Не знайдено плеєрів!";
        nt = "Не знайдено торентів!";
    } else {
        np = "No players was found";
        nt = "No torrents was found";
    }
#endif
    httplib::Client server ("https://anitube.in.ua");
        std::string resp {server.Get(in.substr(in.find("anitube.in.ua")+13), headers)->body};
        //resp = resp.substr(resp.find("]")+8);resp = resp.substr(0,resp.find(",\"")-1);
    unescape(resp);
    std::string orig;
    if (resp.find("<strong>Оригінальна назва:</strong>") == std::string::npos) {
        orig = resp.substr(resp.find("h2")+4);
        orig=orig.substr(0,orig.find("<")-1);
    } else {
        orig = resp.substr(resp.find("<strong>Оригінальна назва:</strong>")+52);
        orig=orig.substr(0,orig.find("<"));
    } //pls move this or...
    unsigned short year = stod(resp.substr(resp.find("a/xfsearch/year/")+16,4));
    std::string sers = getsers(resp);
    if (display) {
       std::cout << orig << " (" << year << "), " << sers << std::endl;
    } else {
        try {
            if (stod(sers) != stod(sers.substr(sers.find("з")+2))) {
               std::cout << "\e[1m" << serii << "\e[0m: " << stod(sers) << "/" << stod(sers.substr(sers.find("з")+2)) << std::endl;
            }
        }
        catch(...) {
            std::cout << "\e[1m" << serii << "\e[0m: " << sers.substr(0,sers.find("(")) << std::endl;
        }
    }
    while (orig.find("/") != std::string::npos) {
        orig.replace(orig.find("/"),1,"∕");
    }
    if (resp.find("team_list") == std::string::npos) {
    //while (voiced.find("https://anitube.in.ua/xfsearch/voiced") != std::string::npos)
//voiced.replace(voiced.find("https://anitube.in.ua/xfsearch/voiced"), 46, "");
     //voiced = voiced.substr(0,voiced.find("https://anitube.in.ua/xfsearch/voiced")) + voiced.substr(voiced.find("https://anitube.in.ua/xfsearch/voiced")+38);
    simplevoiced(resp,1,sounded);
    } else {
        std::string voiced = resp.substr(resp.find("team_list")-19);
        voiced=voiced.substr(0,voiced.find("<style")-1);
        std::regex pattern("<[^>]+>");
        voiced = std::regex_replace(voiced, pattern, ",");
        voiced=voiced.substr(5);
        if (voiced.substr(0,1) == ",") {
            voiced=voiced.erase(0,1);
        }
        while (voiced.find(",,,,,,")!=std::string::npos) {
            voiced.replace(voiced.find(",,,,,,"),6,
#ifdef _WIN32
    std::endl);
#else
    "\n\e[1m");
#endif
        }
        while (voiced.find(",,,,,")!=std::string::npos) {
            voiced.replace(voiced.find(",,,,,"),5,
#ifdef _WIN32
        std::endl);
#else
        "\n\e[1m");
#endif
        }
#ifdef _WIN32
        voiced=un(voiced,",:")
#else
        while (voiced.find(",:")!=std::string::npos) {
            voiced.replace(voiced.find(",:"),1,"\e[0m");
        }
#endif

        while (voiced.find(": ,,")!=std::string::npos) {
            voiced.replace(voiced.find(": ,,"),4,
#ifdef _WIN32
                            ": ");
#else
                            "\e[0m: ");
#endif
        }

        while (voiced.find(",,")!=std::string::npos) {
            voiced.replace(voiced.find(",,")+1,1," ");
        }
        voiced = un(voiced,"  ");
        std::cout << "\e[1m" << voiced.substr(0,voiced.size()-2) << std::endl;
    }
    unsigned short last = 0;
    std::string vids = "novids";
    std::vector<std::string>playername,links;
    unsigned char lastv;
    bool normal = 0;
    //bool plast = 0;
    std::string nnames;
    std::string serstr = resp.substr(resp.find("ій:"));
    serstr = serstr.substr(serstr.find(">")+2);
    if (resp.find("RalodePlayer.init") != std::string::npos) {
        playername = playerD(resp,last,vids);
        lastv=last;
        normal=1;
    } else if (resp.find("playlists-ajax") != std::string::npos) {
        vids = server.Get("/engine/ajax/playlists.php?news_id="+std::to_string(stoi(in.substr(22)))+"&xfield=playlist&user_hash="+resp.substr(resp.find("h = '")+5,40),headers)->body;
        while (vids.find("amp;") != std::string::npos) {
            vids.erase(vids.find("amp;"), 4);
        }
        unescape(vids);
        //vids=vids.substr(0,vids.find("episode-name"))+vids.substr(vids.find("<\\/div><\\/div><\\/div>"));
        //std::cout << "running playerajaxD with" << vids.substr(146,vids.find("/ul")-154);
        last=playerajaxD(vids.substr(146,vids.find("/ul")-154),links);
        lastv=last;
        nnames = vids.substr(vids.find("/div")+54);nnames = nnames.substr(0,nnames.find("/div")-13);
        //plast = (nnames.substr(8,2) == "П");
        //if (plast)while (nnames.find(">MP4") != std::string::npos)nnames.replace(nnames.find(">MP4"), 1, ">Р ");
    } else {
#ifdef _WIN32
        std::wcout << np
#else
        std::cout << "\033[101m" << np << "\033[0m"
#endif
        << std::endl;
    }
    httplib::Headers blank;
    if ((headers != blank) && (qbittorrent.size()+torrserver.size() != 0)) {
        links.resize(lastv+1);
        if (resp.find("t_title\"") != std::string::npos) { // перевірка на факт наявності торентів
            serstr = serstr.substr(0,serstr.find("<h"));
            std::string hv;
            unsigned short dur = 0;
            if (serstr.find("(") != std::string::npos) {
                hv = serstr.substr(serstr.find("(")+1);//char not tested
                if (hv.substr(2,6) == "год") {
                    dur=stod(hv)*60;hv=hv.substr(9,8);
                }
                dur = dur+stod(hv);
                if (hv.find("хв") != std::string::npos) {
                    dur=dur*60;
                }
                //std::cout << stod(dur.substr(dur.find("(")+1));return;
            }
            while (resp.find("t_title\"") != std::string::npos) {
                resp = resp.substr(resp.find("t_title\"")+9);resp=resp.substr(0,resp.find("clr")-218);
                // << torrents.substr(0,torrents.find(":"));
                std::string temp = resp.substr(0,resp.find(":<")); //https://anitube.in.ua/2140-dolya-nch-bitvi-svt-nesknchennih-klinkv-fate-stay-night-unlimited-blade-works-tv-2.html
                std::string name = temp.substr(0,temp.find("|"));
                while (name.find("amp;") != std::string::npos) {
                    name.erase(name.find("amp;"), 4);
                }
                //name.replace(name.find(")")+1, 0, " ");
                //if (name.find("  ") != std::string::npos)name.replace(name.find("  "), 1, "");
                //name.erase(std::find_if(name.rbegin(), name.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), name.end()); // https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
                unsigned short len = name.size();
                if (name.rfind(" ") == --len) {
                    name.erase(len, len+1);
                }
                if (name.find(" ") == 0) {
                    name.erase(0,1);
                }
                while (name.find("&#039;") != std::string::npos) {
                    name.replace(name.find("&#039;"), 6, "'");
                }
                //temp = temp.substr(temp.find("|")+1);
                temp = resp.substr(resp.find("t\">"));
                std::string link=temp.substr(temp.find("href")+6);
                links.push_back(link.substr(0,link.find("\"")));//+5)); навіщо?
                temp = temp.substr(temp.find(":")+1,temp.find("<")-temp.find(":")-1);
                while (temp.find(" ") != std::string::npos) {
                    temp.erase(temp.find(" "), 1);
                }
                std::string sizestr = temp;
                if (temp.find(".") != std::string::npos) {
                    temp.replace(temp.find("."), 1, ",");
                }
                unsigned short c = 8192;
                if (temp.find("G") == std::string::npos) {
                    c=8;
                }
                float size = stof(temp)*c;
                temp = resp.substr(resp.find("<span>")+6);
                ++last;
                unsigned short sers=1;
                try {
                    sers=stod(temp.substr(temp.find("<td>")+4));
                } catch(...){

                }
#ifdef _WIN32
                std::cout << last << ") " << name << ", \033[32m" << stod(temp) << "\033[1;37m/\033[0;31m" << stod(temp.substr(temp.find("<span>")+6)) << "\033[0m, " << stod(temp.substr(temp.find("жено: ")+10)) << " ";
                std::wcout << skachali;
                std::cout << ", " << sizestr << " (" << round(size/sers/dur*10)/10 << "Mbit/"; std::wcout << sec;
                std::cout << ")" << std::endl;}}else std::wcout << nt
#else
                std::cout << last << ") " << name << ", \033[32m" << stod(temp) << "\033[1;37m/\033[0;31m" << stod(temp.substr(temp.find("<span>")+6)) << "\033[0m, " << stod(temp.substr(temp.find("жено: ")+10)) << " " << skachali << ", " << sizestr; if (dur != 0)std::cout << " (" << round(size/sers/dur*10)/10 << "Mbit/" << sec << ")";std::cout << std::endl;
            }
        } else {
            std::cout << "\e[1;30m" << nt << "\e[0m"
#endif
     << std::endl;
        }
    }
    //curl 'https://anitube.in.ua/1886-shteynova-brama-steinsgate-steins-gate.html' -H 'Connection: keep-alive' -H "Cookie:$data" | grep torrent\"
    std::string need;
    short want = -1;
    std::cin >> need;
    //if (vids != "novids")
    if (need.length() > std::to_string(last).length()) {
        want=stod(need.substr(std::to_string(last).length()))-1;
        need=need.substr(0,std::to_string(last).length());
    }
    if (stod(need)>lastv) {
        torrent("https://anitube.in.ua" + links[stod(need)],torrserver,qbittorrent,mpv,want);
    } else {
        if (normal) {
            system(chooseP(res,mpv,stod(serstr),vids+"}",orig,playername,lc,stod(need),over,testfunc).c_str());
        } else {
            //std::cout << vids.substr(vids.find("0_"+std::to_string(short(stod(need)-1))+"_0"));
            std::string tosomes;
            if (vids.find("0_0_0_0") != std::string::npos) {
                tosomes = vids.substr(vids.find("0_0_0_0"));
                tosomes = tosomes.substr(0,tosomes.find("\\/li><\\/ul"));
            }
            vids = vids.substr(vids.find("data-file")+12);
       //std::string toparse;
       //if (!plast){
       //unsigned char i = 1;
       //while (i<stod(need)) {whoclone = whoclone.substr(whoclone.find("\">")+2);++i;} //whoclone is basically who
       //toparse = whoclone.substr(11,2);}
       vids = vids.substr(vids.rfind("http", vids.find("data-id=\\\"0_"+std::to_string(short(stod(need)-1)))));
       std::string cur;
       //if (vids.find("data-id=\\\"0_"+std::to_string(short(stod(need)))) == std::string::npos)
       if (stod(need) == lastv){vids = vids.substr(0, vids.rfind("/li"));cur=vids;}
       else {vids = vids.substr(0, vids.find("\\/li><li data-file", vids.find("data-id=\\\"0_"+std::to_string(short(stod(need))))));
       cur = vids.substr(0,vids.rfind("\\/li"));}
       std::string vidsclone = vids;
       std::vector <std::string> somes;
       vids = vids.substr(vids.find("data-id=\\\"")+10);
       //std::cout << "making " << vids << " somes" << std::endl;
       somes.push_back(vids.substr(0,vids.find("\\")));
       while (vids.find("data-id", vids.find("data-id")+10) != std::string::npos){
       vids = vids.substr(vids.find("data-id=\\\"")+10);
       if (somes.back() != vids.substr(0,vids.find("\\"))) somes.push_back(vids.substr(0,vids.find("\\")));}
       if (CountOccurrences((char*)somes[0].c_str(),(char*)"_")>2){
       std::string tofunc = nnames;
       unsigned char toadd=0;
       if (need != "1"){toadd=CountOccurrences((char*)nnames.substr(0,nnames.find("\"0_"+std::to_string(char(stod(need)-1)))).c_str(),(char*)"data-id");tofunc = nnames.substr(nnames.find("\"0_"+std::to_string(char(stod(need)-1))));}
       tofunc = tofunc.substr(tofunc.find(">")+1);
       if (tofunc.find("\"0_"+need) != std::string::npos){
       tofunc=tofunc.substr(0,tofunc.find("\"0_"+need));
    tofunc=tofunc.substr(0,tofunc.rfind("\\/"));}
       links.resize(0);somes.resize(0);last=playerajaxD(tofunc,links);
       while (true){
           somes.push_back(nnames.substr(0,nnames.find("\\")));
           if (nnames.find("=\\\"") == std::string::npos) break;else
           nnames = nnames.substr(nnames.find("=\\\"")+3);
       }
       std::cin >> need;
       need = std::to_string(stod(need)+toadd);
       unsigned short start = 0;if (stod(need)>1)start=vidsclone.rfind("http",vidsclone.find("\"" + somes[stod(need)-1]));
       unsigned short end = -1;if(stod(need)!=somes.size())end=vidsclone.rfind("\\/li",vidsclone.find("\"" + somes[stod(need)])); //unsigned short int suitable even for https://anitube.in.ua/4110-chainsaw-man.html. Seems like no need to 4-byte
       nnames = vidsclone.substr(start,end-start);
       tosomes = tosomes.substr(tosomes.find(somes[stod(need)-1]+"_"));tosomes = tosomes.substr(0,tosomes.find("<",tosomes.rfind(somes[stod(need)-1]+"_")));
       //std::string tosomesclone = tosomes;
       somes.resize(0);
       bool hear = tosomes.find(">") != tosomes.rfind(">");
       for (unsigned short i = 1;tosomes.find(">") != std::string::npos;++i){
           if (hear) std::cout << i << ") " << tosomes.substr(tosomes.find(">")+1,tosomes.find("<")-tosomes.find(">")-1) << std::endl;
           somes.push_back(tosomes.substr(0,tosomes.find("\\")));
           try{tosomes=tosomes.substr(tosomes.find(">")+15);}catch(...){tosomes="";}try{tosomes=tosomes.substr(tosomes.find(">")+15);}catch(...){} //rewrite it
       }
       if (hear) std::cin >> need; else need="1";
       cur = vidsclone.substr(vidsclone.rfind("http",vidsclone.find(somes[stod(need)-1])));
       if (cur.substr(cur.find("data-id")+10,5) != cur.substr(cur.rfind("data-id")+10,5)){cur = cur.substr(0,cur.rfind("\\/li",cur.find(somes[stod(need)])));}
       } //stod(need) != somes.size()
       //if (somes.size()+tosomes.size() > 0){
       //if (tosomes.size() == 0){
       //    if (somes.size() != 1){
       //unsigned char i = 0;
       //while (i < somes.size()){std::string temp = nnames.substr(nnames.find(somes[i]));//if (temp.substr(temp.find(">")+12,2) == "GG")std::cout << "\e[9m\e[1;30m";
       //    std::cout << i+1 << ") " << temp.substr(temp.find(">")+1,temp.find("<")-temp.find(">")-1) << std::endl
       //              //<< "\e[0m"
       //                 ;++i;}
       //std::cin >> need;} else need="1";
       //cur = cur.substr(cur.rfind("http", cur.find(somes[stod(need)-1])));
       //if (stod(need) != somes.size())cur = cur.substr(0,cur.rfind("\\/li",(cur.find(somes[stod(need)]))));} можливо, цей код замінює щось вище... Зараз він зайвий
       //if (plast) {toparse = cur.substr(cur.find("=")+3); toparse = nnames.substr(nnames.find(toparse.substr(0,toparse.find("\\")))); toparse = toparse.substr(toparse.find("Р")+3,2);}
       spaces(CountOccurrences((char*)cur.c_str(),(char*)"data-id")-1);
       std::string mpvargs;
       std::ofstream m3u;
       m3u.open (std::filesystem::temp_directory_path().string()+"/cvd.m3u");
       m3u << "#EXTM3U" << std::endl;
       while (cur.find("<li") != std::string::npos){
       m3u << "#EXTINF:-1, " << cur.substr(cur.find(">")+1,cur.find("<")-cur.find(">")-1) << std::endl << parse(cur.substr(0,cur.find("\\\"")),res,mpvargs,mpv,lc,testfunc) << std::endl;
       std::cout << '|' << std::unitbuf;
       cur = cur.substr(cur.find("e=\\\"")+4);}
       m3u << "#EXTINF:-1, " << cur.substr(cur.find(">")+1,cur.find("<")-cur.find(">")-1) << std::endl << parse(cur.substr(0,cur.find("\\\"")),res,mpvargs,mpv,lc,testfunc) << std::endl << "#EXT-X-ENDLIST";
   m3u.close();
   std::cout << '\n';
   system((mpv + " '" + std::filesystem::temp_directory_path().string()+"/cvd.m3u' " + mpvargs).c_str());
    //}
    }}}
std::string get_dle(httplib::Headers headers){std::string resp = {httplib::Client ("https://anitube.in.ua").Get("/", headers)->body};return resp.substr(resp.find("dle_login_hash")+18,40);}
void anilibria(std::string req,std::string in,std::string qbittorrent,std::string addr,std::string darklibria,std::string dark,std::string mpv,std::string show_hidden,std::string lc,bool warn,
               #ifdef _WIN32
               std::wstring sounded,std::wstring sec,std::wstring skachali,std::wstring net,std::wsrting serii
               #else
               std::string sounded,std::string sec,std::string skachali,std::string net,std::string serii
               #endif
               ){
    std::string resp = {httplib::Client ("http://api.anilibria.tv").Get(req)->body};
                while (resp.find("\\\"") != std::string::npos)
            resp.replace(resp.find("\\\""), 2, "cvd");
                while (resp.find("\"alternative\":null") != std::string::npos)
              resp.replace(resp.find("\"alternative\":null"), 18, "\"alternative\":\"null\"");
            unsigned short last;
            unsigned short all = CountOccurrences((char*)resp.c_str(), (char*)"\"id\"");
            std::string year=resp.substr(resp.find("year")+6);
            if (all > 1) {
        for (unsigned short i = 1; i != all+1; i++){std::cout << i << ") ";
            int comma = 0;
            int hit;
    unsigned char ens = 0;
        for(int also = 0; also < resp.length(); also++) {
            if(resp[also] == '"') {
                comma++;
                if (comma == 5 + 20*(i-1))hit=also;
    if (comma == 9 + 20*(i-1)){hit=also;ens=4;}
            }else if (comma == ens + 6 + 20*(i-1)) {
    if (ens != 0) std::cout << " / ";
                std::string sub = resp.substr(hit+1, also-hit-2);
            while (sub.find("cvd") != std::string::npos)//{
            sub.replace(sub.find("cvd"), 3, "\"");//brokensub = 1;}
            std::cout << sub;
                if (ens != 0){//std::cout << "https://anitube.in.ua/f/l.title=" + normalize(sub) + "/r.year=" + year.substr(0,4) + ";" + year.substr(0,4);
                    std::cout << " (" << year.substr(0,4) << ")" << std::endl;year=year.substr(year.find("year")+6);break;}
            }
        }
        last=i;
    };
    std::cout << "? ";std::cin >> in;
    for (unsigned short f = 0; f<stod(in)-1;++f) {
        resp = resp.substr(resp.find("id\":")+4);
    }
    std::string id = resp.substr(resp.find("id\":")+4,4);
    //std::cout<<id; //} взагалі буває? під чим я це писав?
    if ((id.substr(3) == "}")||(id.substr(3)==",")) {
        id=id.substr(0,3);
    }
    AniLibriaById(id,qbittorrent,addr,darklibria,dark,mpv,show_hidden,lc,sounded,sec,skachali,serii,warn);
    } else if (all == 0) {
#ifdef _WIN32
        std::wcout << net << std::endl;
#else
        std::cout << net << std::endl;
#endif
    } else {
        unsigned short comma = 0;
        for(int also = 0; also < resp.length(); also++) {
            if(resp[also] == '"') {
                comma++;
                if (comma == 16) {//Нє, ну тут точно колись } було, воно працювало
                    if ((resp[also+5] == '}')||(resp[also+5] == ',')) {
                        AniLibriaById(resp.substr(also+2, 3),qbittorrent,addr,darklibria,dark,mpv,show_hidden,lc,sounded,sec,skachali,serii,warn);
                    } else {
                        AniLibriaById(resp.substr(also+2, 4),qbittorrent,addr,darklibria,dark,mpv,show_hidden,lc,sounded,sec,skachali,serii,warn);
                        break;
                    }
                }
            }
        }
    }
}

unsigned short anitubesearch(std::vector<std::string> &links, unsigned short i, unsigned short all,std::string req, std::string &toecho) {
    std::string in;
    while (i != all) {
        req=req.substr(req.find("href")+6);
        if (req.substr(1,1) == "\\") {
            return stod(req.substr(req.find(" (")+2));
            return 0;
        } else {
            //links.push_back(req.substr(req.find("a\\/")+3,req.find("\\\">")-req.find("a\\/")-3));
            links.push_back(req.substr(26,req.find("\\\">")-26));
            in=req.substr(req.find(">",req.find(">")+1)+6);
            in=in.substr(0,in.find(")")+1);in=un(in,"\\/");in=un(in,"  ");
            ++i;
            toecho = toecho + std::to_string(i) + ")" + in  + " " + getsers(req) + "\n";
        }
    }
    return 0;
}

std::string anitubesite(std::string aniresp, std::vector<std::string> &links, short limit,
#ifdef WIN32
std::wstring
#else
                 std::string
#endif
                 sounded) {
    unsigned short i =0;
    while (i<11 && i<limit) {
        ++i;
        aniresp=aniresp.substr(aniresp.find("story_c\"")+39);
        links.push_back(aniresp.substr(0,aniresp.find("\"")));
        std::string temp = aniresp.substr(aniresp.find("l\">")+3);
        std::string name = temp.substr(0,temp.find("<"));
        std::string date = temp.substr(temp.find("y_d")+13);
        std::string datelast = date.substr(date.find("b")+2,6);
        std::string dateultralast = date.substr(date.find("l\">")+3);dateultralast = dateultralast.substr(0,dateultralast.find("<"));
        while (dateultralast.find("amp;") != std::string::npos) {
            dateultralast.erase(dateultralast.find("amp;"), 4);
        }
        std::string type = date.substr(date.find("tit")+7);
        std::string shorttype = type.substr(type.find(">",40)+1,3);
        type = type.substr(0,type.find("\""));
        date = date.substr(0,date.find("<"));
        std::cout << i << ") " << name << " \e[3";
        if (shorttype == "DUB") {
            std::cout << 2;
        } else if (shorttype == "D+S") {
            std::cout << 5;
        } else if (shorttype == "SUB") {
            std::cout << 3;
        }
        std::cout << "m" << type << " \e[90m" << date << datelast;
        if (dateultralast != "Перегляд") {
            std::cout << " " << dateultralast; std::cout <<"\e[0m " << getsers(aniresp.substr(0,aniresp.find("_ra")));
            simplevoiced(aniresp.substr(0,aniresp.find("_ra")),0,sounded);
            std::cout << std::endl;
        }
    }
    std::cin >> i;
    return links[i-1];
    //std::cout << aniresp.substr(0,aniresp.find("v class=\"n")-66);
}

int main(int args,char **arg) {
#ifdef _WIN32
    SetConsoleTitle("cvd++: But the cat murmur");
#else
#endif
    setlocale(LC_ALL, "");
    std::string lc = std::string(setlocale(LC_CTYPE, NULL)).substr(0,3);
    if (lc.find("_") == 2) {
        lc=lc.substr(0,2);
    }
#ifdef _WIN32
    std::wstring
#else
    std::string
#endif
    oldcfg,oldcfgsoft,oldsoft,sounded,net,sec,skachali,nc,testfunc,serii,found;
    if (lc == "uk" || lc == "Ukr" || lc == "ru" || lc == "Rus") {
#ifdef _WIN32
    oldcfg = L"Файл конфігурації застарів! Продовжувати роботу неможливо.";
    oldcfgsoft = L"Файл конфігурації застарів! Нові функції можуть бути недоступні.";
    oldsoft = L"Версія програми застаріла! Замініть поточний файл конфигураії на відповідний для цієї версії або оновіть програму.";
    sounded = L"Озвучували";
    net = L"Нема результатів...";
    sec = L"сек";skachali=L"завантажили";
    nc = L"Файл конфігурації не знайдено!";
    testfunc = L"Будь ласка, повідомте мені про те, що ви зараз робите, це допоможе програмі! Код ";
    found = L"Знайдено більше п'яти релізів, оберіть один або натисність 6, щоб показати всі ";
#else
    oldcfg = "Файл конфігурації застарів! Продовжувати роботу неможливо.";
    oldcfgsoft = "Файл конфігурації застарів! Нові функції можуть бути недоступні.";
    oldsoft = "Версія програми застаріла! Замініть поточний файл конфигураії на відповідний для цієї версії або оновіть програму.";
    //sibnet = "Sibnet не підтримуєтся та його підтримка не запланована. Оберіть інший плеєр";
    sounded = "Озвучували";
    net = "Нема результатів...";
    sec = "сек";skachali="завантажили";
    nc = "Файл конфігурації не знайдено!";
    testfunc = "Будь ласка, повідомте мені про те, що ви зараз робите, це допоможе програмі! Код ";
    serii="Серії";
    found = "Знайдено більше п'яти релізів, оберіть один або натисність y, щоб показати всі ";
#endif
    } else
//if (lc == "ru" || lc == "Rus"){
//oldcfg = "Конфиг устарел! Продолжать работу невозможно.";
//oldcfgsoft = "Конфиг устарел! Новые функции недоступны.";
//oldsoft = "Версия программы устарела! Замените текущий конфиг на подходящий для этой версии или обновите программу.";
//sibnet = "Sibnet не поддерживается и не собирается. Выберите другой плеер";
//sounded = "Озвучивали";
//net = "Нет результатов...";
//sec = "сек";}else
    {
#ifdef _WIN32
    oldcfg = L"Confid is too old! Exiting...";
    oldcfgsoft = L"Confid is too old! New features might be exclusive.";
    oldsoft = L"Software is too old! Downgrade config or update software.";
    sounded = L"Voiced by";
    net = L"Nothing was found.";
    sec = L"sec";skachali=L"downloaded";
    nc = L"No user config file!";
    testfunc = L"Please, report about what you are doing rigth now. That will help development. Code ";
    serii = L"Episodes";
    found = L"More then 5 releases found. Choose one or hit y to show all ";
#else
    oldcfg = "Confid is too old! Exiting...";
    oldcfgsoft = "Confid is too old! New features might be exclusive.";
    oldsoft = "Software is too old! Downgrade config or update software.";
    //sibnet = "Sibnet is unsuppored and not planned. Try another player";
    sounded = "Voiced by";
    net = "Nothing was found.";
    sec = "sec";skachali="downloaded";
    nc = "No user config file!";
    testfunc = "Please, report about what you are doing rigth now. That will help development. Code ";
    serii = "Episodes";
    found = "More then 5 releases found. Choose one or hit y to show all ";
#endif
    }
#ifdef unix
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
#endif
    // string line;
    // std::ifstream myfile ("/tmp/1");
    // if (myfile.is_open()) {
    //     while ( getline (myfile,line) ) {
    //       std::cout << line << '\n';
    //     }
    //     myfile.close();
    // }
    std::string limit = "50";
    std::string qbittorrent,addr,darklibria,dark,mpv,show_hidden;
    httplib::Headers headers;
    unsigned short res = 720;
    bool warn = 1;
    bool over = 0;
    bool display = 1;
    bool norusso = 1;
    bool autoexpand = 1;
    bool forceexpand = 0;
    // std::ifstream is RAII, i.e. no need to call close https://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c
#ifdef unix
    std::ifstream cFile(homedir + std::string("/.config/cvd/cvd.conf"));
#endif
#ifdef _WIN32
    std::ifstream cFile(getenv("APPDATA") + std::string("/cvd/cvd.conf"));
#endif
    if (cFile.is_open()) {
        std::string line;
        while (getline(cFile, line)) {
//            #ifdef unix
//                        line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end()); //Program Files
//            #endif
            if (line[0] == '#' || line.empty()) {
                continue;
            }
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            //Custom coding
            if (name == "limit") {
                limit = value;
            } else if (name == "player") {
                if (value == "mpv") {
#ifdef unix
                    mpv = "mpv --write-filename-in-watch-later-config --watch-later-directory=~/.config/cvd/mpv_watch_later";
#endif
#ifdef _WIN32
                        mpv = "mpv --write-filename-in-watch-later-config --watch-later-directory=" + std::string(getenv("APPDATA")) + "/cvd/mpv_watch_later";
#endif
                } else {
                mpv = value;
                }
            } else if (name == "darklike") {
                dark = value;
            } else if (name == "libriamirror") {
                darklibria = value;
            } else if (name == "torrserver") {
                addr = value;
            } else if (name == "torrent") {
                qbittorrent = value;
            } else if (name == "iregretnothing") {
                show_hidden = value;
            } else if (name == "res") {
                res = stod(value)/360;
            } else if (name == "anitube.in.ua") {
                headers = {{"Cookie", value}};
            } else if (name == "warn") {
                warn = stod(value);
            } else if (name == "over") {
                over = stod(value);
            } else if (name == "display") {
                display = stod(value);
            } else if (name == "norusso") {
                norusso = stod(value);
            } else if (name == "autoexpand") {
                autoexpand = stod(value);
            } else if (name == "confverA") {
                if (stod(value) < 1) {
#ifdef _WIN32
                    std::wcout
#else
                    std::cout
#endif
                    << oldcfg << std::endl;
                    return 1;
                }
            } else if (name == "confverB") {
                if (stod(value) < 3) {
#ifdef _WIN32
                    std::wcout
#else
                    std::cout
#endif
                    << oldcfgsoft << std::endl;
                }
            } else if (name == "confverC") {
                if (stod(value) > 1) {
#ifdef _WIN32
                    std::wcout
#else
                    std::cout
#endif
                    << oldsoft << std::endl;
                    return 1;
                }
            }
        }
    } else {
#ifdef _WIN32
        std::wcerr
#else
        std::cerr
#endif
        << nc << std::endl;
    }
    //std::future<std::string> dle_login_hash = std::async (get_dle,headers);
    std::string in,req;
    if (IsPathExist("/data/data/com.termux")) {
        std::cout << "you're in termux. This is experemntal. The program WILL NOT fully work." << std::endl << "Continue?(y/n)";std::cin >> in;
        if (in == "n") {
            std::cout << "'No' detected, exiting.." << std::endl;
            return 0;
        } else {
            system("/data/data/com.termux/files/usr/bin/termux-api-start >/dev/null&");
        }
    }
    if (args > 1) {
        for (int i=0; i < strlen(arg[1]); i++) {
            in=in+arg[1][i];
        }
    } else {
        std::cin >> in;
    }
    if (args > 2) {
        limit=arg[2];
    }
    if (is_number(in)) {
        if (stod(in) > 380 && stod(in) < 10000) {
            AniLibriaById(in,qbittorrent,addr,darklibria,dark,mpv,show_hidden,lc,sounded,sec,skachali,serii,warn);
        }
    } else {
        if (in == "showlast") {
            if (norusso) {
                req="/anime";
            } else {
                req="/v2/getChanges?filter=names,id,season.year&show_hidden=" + show_hidden + "&limit=" + limit;
            }
        } else
            if (in.find("anitube.in.ua") != std::string::npos) {
            anitube(in,mpv,lc,res,headers,addr,qbittorrent,over,display,skachali,sec,sounded,testfunc,serii);
            return 0;
        } else if (in.find("csst.online") != std::string::npos || in.find("secvideo") != std::string::npos || in.find("onvi.cc") != std::string::npos || in.find("monstr") != std::string::npos) {
            system((mpv + " " + monster(in,res-1,testfunc)).c_str());
            return 0;
        } else if (in.find("https://www.mp4upload.com") != std::string::npos) {
            std::string mpvargs;system((mpv + " " + parse(in,res,mpvargs,mpv,lc,testfunc) + " " + mpvargs).c_str());
            return 0;
        } else if ((in.find("://tortuga.wtf") != std::string::npos) || (in.find("https://ashdi.vip") != std::string::npos) || (in.find("https://ashdi.vip") != std::string::npos)) {
            system((mpv + " " + veryuniversal(in)).c_str());
            return 0;
        } else if (in.find("https://www.udrop.com") != std::string::npos) {
            system((mpv + " " + udrop(in)).c_str());
            return 0;
        } else if (in.find("/videos/embed/") != std::string::npos) {
            system((mpv + " " + realpeer(in,res)).c_str());
            return 0;
        } else if (in.find("https://moonanime.art") != std::string::npos) {
            system((mpv + " " + moon(in,res)).c_str());
            return 0; // add ufdub
        } else if (norusso) {
            req=in;
        } else {
            req="/v2/searchTitles?filter=names,id,season.year&show_hidden=" + show_hidden + "&limit=" + limit + "&search=" + url_encode(in);
        }
        if (norusso) {
            httplib::Client tub("https://anitube.in.ua");
            tub.set_follow_location(true);
            std::string aniresp;
            std::vector<std::string> links;
            unsigned short i=0;
            if (in == "showlast") {
                anitube(anitubesite(tub.Get(req)->body,links,stod(limit),sounded),mpv,lc,res,headers,addr,qbittorrent,over,0,skachali,sec,sounded,testfunc,serii);
            } else {
                req = tub.Post("/engine/lazydev/dle_search/ajax.php",headers,"story="+req+"&dle_hash="+get_dle(headers), "application/x-www-form-urlencoded")->body;
                req=req.substr(0,req.find("style>"));
                unsigned char all=CountOccurrences((char*)req.c_str(),(char*)"href");
                if (all == 1) {
                    anitube("https://anitube.in.ua/"+req.substr(req.find("a\\/")+3,req.find("\\\">")-req.find("a\\/")-3),mpv,lc,res,headers,addr,qbittorrent,over,display,skachali,sec,sounded,testfunc,serii);
                } else {
                    std::string toecho;
                    if (!forceexpand) {
                        all = anitubesearch(links,i,all,req,toecho);
                    }
                    if (all != 0) {
                        if (!autoexpand) {
                            std::cout << toecho << "\e[5m\e[93m" << found << "\e[0m" << all << std::endl;
                            std::cin >> toecho;
                        }
                    } else {
                        std::cout << toecho;
                    }
                    if (is_number(toecho) && stoi(toecho) < 6) {
                        anitube("https://anitube.in.ua/"+links[i-1],mpv,lc,res,headers,addr,qbittorrent,over,0,skachali,sec,sounded,testfunc,serii);
                    } else {
                        aniresp = tub.Post("/","do=search&story="+in,"application/x-www-form-urlencoded")->body;
                        all=CountOccurrences((char*)aniresp.c_str(),(char*)"story_c\"");
                        if (all > stoi(limit)) {
                            all=stoi(limit);
                        }
                        anitube("https://anitube.in.ua/"+anitubesite(aniresp,links,all,sounded),mpv,lc,res,headers,addr,qbittorrent,over,0,skachali,sec,sounded,testfunc,serii);
                    }
//                  curl https://anitube.in.ua --data-raw 'do=search&story=naruto'
                }
            }
        } else {
            anilibria(req,in,qbittorrent,addr,darklibria,dark,mpv,show_hidden,lc,warn,sounded,sec,skachali,net,serii);
        }
    }
    return 0;
}
            //curl 'https://www.anilibria.tv/public/login.php' --data-raw 'mail=van.a.gribanov@gmail.com&passwd=****&fa2code=' | jq .sessionId
            //curl 'https://toloka.to/tracker.php?nm=chainsaw' -H 'Cookie: toloka_sid=***' | sed ':a;N;$!ba;s/\n/cvdnewlinecvd/g' | sed 's/.*<\/th>'//g | sed s/cvdnewlinecvd/\\n/g
            // cookie=$(curl 'https://toloka.to/login.php' --data-raw 'username=van.a.gribanov@gmail.com&password=***&autologin=on&login=Вхід' -c - | grep toloka_sid | cut -c47-)

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include <cqcppsdk/cqcppsdk.h>

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

int buyid;
int sellid;
bool on = true;
int give;
int64_t give_to;
int many;
int eat_id;
int eat_many;
int64_t trans_to;
double trans_many;
int t1=0,t2=0,t3=0;

struct node {
  string username;
  double money;
  map<string, double> milk;
  double ware;
  int flag;
  int bak;
  int init;
  int lv;
  int eat;
  double weight;
  node()
      : username(username), money(money = 1000), milk(milk), ware(ware = 100),
        flag(flag = 0), bak(bak = 1), lv(lv = 0), init(init = 1), eat(eat),
        weight(weight = 50) {}
};

map<int64_t, node> account;
map<string, string> unit;
map<string, double> energy;
vector<pair<double, string>> price;
string admin;
double ad;

double read(string s) {
  int pos = 0;
  double x = 0;
  double tmp = 1.0;
  int fir = 1;
  while (s[pos] && (s[pos] < '0' || s[pos] > '9'))
    pos++;
  while (s[pos] && (s[pos] >= '0' && s[pos] <= '9')) {
    x = x * 10 + s[pos] - '0';
    pos++;
  }
  if (s[pos] == '.' && fir) {
    fir = 0;
    pos++;
    while (s[pos] && (s[pos] >= '0' && s[pos] <= '9')) {
      x += (tmp / 10.0) * (double)(s[pos] - '0');
      pos++;
    }
  }
  return x;
}

int64_t sread(string s) {
  int64_t pos = 0;
  int64_t x = 0;
  while (s[pos] && (s[pos] < '0' || s[pos] > '9'))
    pos++;
  while (s[pos] && (s[pos] >= '0' && s[pos] <= '9')) {
    x = x * 10ll + s[pos] - '0';
    pos++;
  }
  return x;
}

void save(int64_t id) {
  ofstream out(to_string(id));
  out << account[id].money << " " << account[id].lv << " " << account[id].ware
      << " " << account[id].weight << endl;
  for (auto i : price) {
    out << i.second << " " << account[id].milk[i.second] << endl;
  }
  out.close();
}

void load(int64_t id) {
  ifstream in(to_string(id));
  in >> account[id].money >> account[id].lv >> account[id].ware >>
      account[id].weight;
  string name;
  while (in >> name) {
    int p;
    in >> p;
    account[id].milk[name] = p;
  }
  in.close();
}

void readprice() {
  ifstream in("price.txt");
  vector<pair<double, string>> tmp;
  string name;
  while (in >> name) {
    double p;
    string un;
    double en;
    in >> p >> un >> en;
    unit[name] = un;
    energy[name] = en;
    tmp.emplace_back(make_pair(p, name));
  }
  in.close();
  price = tmp;
}

void writeprice() {
  ofstream out("price.txt");
  for (auto i : price) {
    out << i.second << " " << i.first << " " << unit[i.second] << " "
        << energy[i.second] << endl;
  }
  out.close();
}

void clo() {
  srand((unsigned)time(NULL));
  while (on) {
    int rd = rand();
    double p;
    int len = price.size();
    for (int i = 0; i < len; ++i) {
      int tmp = (rd % 40 + rand() % 40) % 40;
      tmp += 80;
      p = (double)tmp / 100.0;
      price[i] = make_pair(price[i].first * p, price[i].second);
    }
    _sleep(3600000);
  }
}

CQ_INIT {
  on_enable([] {
    readprice();
    clo();
    logging::info("启用", "插件已启用");
  });

  on_private_message([](const PrivateMessageEvent &e) {
    /*
    try {
        auto msgid = send_private_message(e.user_id, e.message); // 直接复读消息
        logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " +
    to_string(msgid)); send_message(e.target, MessageSegment::face(111) +
    "这是通过 message 模块构造的消息~"); // 使用 message 模块构造消息 } catch
    (ApiError &e) { logging::warning("私聊", "私聊消息复读失败, 错误码: " +
    to_string(e.code));
    }

   struct tm *local;
   time_t now;
   now=time(NULL);
   local = localtime(&now);
   send_message(e.target,to_string(local->tm_hour)+"
   "+to_string(local->tm_min)+" "+to_string(local->tm_sec));
   */
  });

  on_message([](const MessageEvent &e) {
    // logging::debug("消息", "收到消息: " + e.message + "\n实际类型: " +
    // typeid(e).name());
  });

  on_group_message([](const GroupMessageEvent &e) {
    static const set<int64_t> ENABLED_GROUPS = {933742633, 1055581351};
    if (ENABLED_GROUPS.count(e.group_id) == 0)
      return; // 不在启用的群中, 忽略

    struct tm *local;
    time_t now;
    now = time(NULL);
    local = localtime(&now);
    int h=local->tm_hour;
    int m=local->tm_min;
    int s=local->tm_sec;
    int d=local->tm_mday;
    if(h==23){
      if(t1!=d&&m<50&&m>=30){
        send_message(e.target,"睡觉觉小贴士:\n🐏🐱奶吧还有三十分钟打烊,请客人们注意时间,保证良好睡眠~");
        t1=d;
      }
      else if(t2!=d&&m>=50){
        send_message(e.target,"睡觉觉小贴士\n🐏🐱奶吧将于十分钟内打烊,感谢客人们的惠顾~明天再见~");
        t2=d;
      }
    }
    else if(h==13){
      if(t3!=d){
        send_message(e.target,"睡觉觉小贴士\n🐏🐱奶吧午休时间到啦~");
        t3=d;
      }
    }

    try {
      /*
      send_message(e.target, e.message); // 复读
      auto mem_list = get_group_member_list(e.group_id); // 获取群成员列表
      string msg;
      for (auto i = 0; i < min(10, static_cast<int>(mem_list.size())); i++) {
          msg += "昵称: " + mem_list[i].nickname + "\n"; // 拼接前十个成员的昵称
      }
      send_group_message(e.group_id, msg); // 发送群消息
      */
      string msg = e.message;
      int64_t id = e.user_id;

      if (account[id].flag == -4) {
        account[id].flag = 0;
        send_message(e.target, "Success");
        energy[admin] = read(msg);
      }

      if (account[id].flag == -3) {
        send_message(e.target, "energy:");
        unit[admin] = msg;
        account[id].flag = -4;
      }

      if (account[id].flag == -2) {
        send_message(e.target, "unit:");
        ad = read(msg);
        price.emplace_back(make_pair(ad, admin));
        account[id].flag = -3;
      }

      if (account[id].flag == -1) {
        send_message(e.target, "price:");
        admin = msg;
        account[id].flag = -2;
      }
      if (msg[0] == '#') {
        logging::debug("Debug", "Access" + to_string(account[id].flag));
        if (account[id].bak) {
          load(id);
          account[id].bak = 0;
        }

        if (msg == "#stop") {
          on = false;
        }

        if (msg == "#start") {
          on = true;
          clo();
        }

        if (msg == "#query") {
          send_message(e.target, to_string(on));
        }

        if (msg == "#admin") {
          send_message(e.target, "Admin Access\nname:");
          account[id].flag = -1;
        }

        if (msg == "#体重") {
          send_message(e.target, "你当前的体重是" +
                                     to_string((int)account[id].weight) + "kg");
        }

        if (msg == "#退出") {
          send_message(e.target, "退出成功!");
          account[id].flag = 0;
        }

        if (msg == "#奶吧") {
          send_message(e.target, "欢迎来到🐏🐱奶吧\n"
                                 "使用以下指令进行操作\n"
                                 "#查询\n"
                                 "#操作\n"
                                 "#退出");
          account[id].flag = 0;
        }

        if (msg == "#查询") {
          send_message(e.target, "--🐏🐱奶吧--\n"
                                 "请输入您要查询的内容\n"
                                 "#当前物价\n"
                                 "#账户余额\n"
                                 "#仓库状态\n"
                                 "#体重\n"
                                 "#退出");
          account[id].flag = 0;
        }

        if (msg == "#操作") {
          send_message(e.target, "--🐏🐱奶吧--\n"
                                 "请输入您要进行的操作\n"
                                 "#购入\n"
                                 "#售出\n"
                                 "#给予\n"
                                 "#扩容\n"
                                 "#转账\n"
                                 "#吃\n"
                                 "#退出");
          account[id].flag = 0;
        }

        if (msg == "#当前物价" || msg == "#物价" || msg == "#奶价") {
          string res = "正在查询当前奶价,每小时更新\n";
          int ID = 0;
          for (auto i : price) {
            res += to_string(++ID) + "." + i.second + "->" +
                   to_string(i.first) + "🐱币/" + unit[i.second] + "\n";
          }
          send_message(e.target, res);
          account[id].flag = 0;
        }

        if (msg == "#账户余额" || msg == "#余额") {
          send_message(e.target, "您的账户余额为" +
                                     to_string((int)account[id].money) + "🐱币");
          account[id].flag = 0;
        }

        if (msg == "#仓库状态" || msg == "#仓库" || msg == "#库存") {
          string res = "您当前持有:\n";
          for (auto i : price) {
            if (account[id].milk[i.second] > 0)
              res += i.second + "->" +
                     to_string((int)account[id].milk[i.second]) +
                     unit[i.second] + "\n";
          }
          send_message(e.target, res);
          account[id].flag = 0;
        }

        if (account[id].flag == 11) {
          double x = read(msg);
          double up = 0;
          for (auto i : price) {
            up += account[id].milk[i.second];
          }
          if (x * price[buyid].first > account[id].money) { // buy failed
            send_message(e.target, "操作失败!\n失败原因:余额不足");
          } else if (x > account[id].ware - up) {
            send_message(e.target, "操作失败!\n失败原因:容量不足");
          } else {
            account[id].money -= x * price[buyid].first;
            account[id].milk[price[buyid].second] += x;
            send_message(
                e.target,
                "操作成功!\n购入" + price[buyid].second + to_string((int)x) +
                    unit[price[buyid].second] + "\n当前持有" +
                    price[buyid].second +
                    to_string((int)account[id].milk[price[buyid].second]) +
                    unit[price[buyid].second]);
          }
          account[id].flag = 0;
        }

        if (account[id].flag == 1) {
          int x = (int)read(msg);
          buyid = x - 1;
          int len = price.size();
          if (buyid >= len || buyid < 0) {
            send_message(e.target, "操作失败!\n失败原因:该ID不存在");
            account[id].flag = 0;
          } else {
            send_message(e.target, "请输入#+要买入的商品数量:");
            account[id].flag = 11;
          }
        }

        if (msg == "#购入" || msg == "#买入" || msg == "买进") {
          string res = "正在查询当前奶价,每小时更新\n";
          int ID = 0;
          for (auto i : price) {
            res += to_string(++ID) + "." + i.second + "->" +
                   to_string(i.first) + "🐱币/" + unit[i.second] + "\n";
          }
          send_message(e.target, res);
          send_message(e.target, "请输入#+要买入的商品ID:");
          account[id].flag = 1;
        }

        if (account[id].flag == 21) {
          double x = read(msg);
          if (x > account[id].milk[price[sellid].second]) {
            send_message(e.target, "操作失败!\n失败原因:库存不足");
            account[id].flag = 0;
          } else {
            account[id].money += price[sellid].first * x;
            account[id].milk[price[sellid].second] -= x;
            send_message(e.target,
                         "操作成功!\n售出" + price[sellid].second +
                             to_string((int)x) + unit[price[sellid].second] +
                             "\n当前持有💰" + to_string((int)account[id].money) +
                             "🐱币");
          }
          account[id].flag = 0;
        }

        if (account[id].flag == 2) {
          int x = (int)read(msg);
          sellid = x - 1;
          int len = price.size();
          if (sellid >= len || sellid < 0) {
            send_message(e.target, "操作失败!\n失败原因:该ID不存在");
            account[id].flag = 0;
          } else {
            send_message(e.target, "请输入#+要售出的商品数量:");
            account[id].flag = 21;
          }
        }

        if (msg == "#售出" || msg == "#出售" || msg == "#卖出") {
          int ID = 0;
          string res = "库存\n";
          for (auto i : price) {
            ID++;
            if (account[id].milk[i.second] > 0) {
              res += to_string(ID) + "." + i.second + "->" +
                     to_string((int)account[id].milk[i.second]) +
                     unit[i.second] + "\n";
            }
          }
          send_message(e.target, res);
          send_message(e.target, "请输入#+要售出的商品ID:");
          account[id].flag = 2;
        }

        if (account[id].flag == 3) {
          if (msg == "#确认") {
            if (account[id].money >= pow(2, account[id].lv) * 5000) {
              send_message(e.target, "操作成功!\n当前仓库等级为" +
                                         to_string(account[id].lv + 1) +
                                         "级\n仓库容量为" +
                                         to_string((int)account[id].ware) +
                                         "升");
              account[id].money -= pow(2, account[id].lv) * 5000;
              account[id].ware *= 1.5;
              account[id].lv++;
            } else {
              send_message(e.target, "操作失败!\n失败原因:余额不足");
            }
          } else {
            send_message(e.target, "已退出扩容!");
          }
          account[id].flag = 0;
        }

        if (msg == "#扩容") {
          send_message(e.target,
                       "您当前的仓库等级为" + to_string(account[id].lv + 1) +
                           ",升级需要" +
                           to_string((int)pow(2, account[id].lv) * 5000) +
                           "🐱币\n再次输入“#确认”以确认");
          account[id].flag = 3;
        }

        if (account[id].flag == 411) {
          many = (int)read(msg);
          account[id].flag = 0;
          if (many > account[id].milk[price[give - 1].second]) {
            send_message(e.target, "赠送失败!\n失败原因:库存不足");
          } else {
            account[id].milk[price[give - 1].second] -= many;
            account[give_to].milk[price[give - 1].second] += many;
            send_message(e.target, "成功赠送" + to_string((int)many) +
                                       unit[price[give - 1].second] +
                                       price[give - 1].second);
            save(give_to);
          }
        }

        if (account[id].flag == 41) {
          give = (int)read(msg);
          send_message(e.target, "请输入#+要赠送的数量");
          account[id].flag = 411;
        }

        if (account[id].flag == 4) {
          give_to = sread(msg);
          logging::debug("Debug", to_string(give_to));
          send_message(e.target, "请输入#+赠送物品ID:");
          account[id].flag = 41;
        }

        if (msg == "#给" || msg == "#给予" || msg == "#送" || msg == "#赠送") {
          int ID = 0;
          string res = "库存\n";
          for (auto i : price) {
            ID++;
            if (account[id].milk[i.second] > 0) {
              res += to_string(ID) + "." + i.second + "->" +
                     to_string((int)account[id].milk[i.second]) +
                     unit[i.second] + "\n";
            }
          }
          send_message(e.target, res);
          send_message(e.target, "请输入#+要赠送的用户的qq:");
          account[id].flag = 4;
        }

        if (account[id].flag == 51) {
          eat_many = (int)read(msg);
          account[id].flag = 0;
          if (account[id].milk[price[eat_id].second] < eat_many) {
            send_message(e.target, "食用失败!\n失败原因:库存不足");
          } else {
            account[id].milk[price[eat_id].second] -= eat_many;
            account[id].weight +=
                (double)eat_many * energy[price[eat_id].second] / 1000.0;
            send_message(e.target,
                         "成功!\n食用了" + to_string(eat_many) +
                             unit[price[eat_id].second] + price[eat_id].second +
                             ",你当前的体重是" +
                             to_string((int)account[id].weight) + "kg");
          }
        }

        if (account[id].flag == 5) {
          eat_id = (int)read(msg) - 1;
          send_message(e.target, "请输入#+要食用的数量");
          account[id].flag = 51;
        }

        if (msg == "#吃") {
          int ID = 0;
          string res = "库存\n";
          for (auto i : price) {
            ID++;
            if (account[id].milk[i.second] > 0) {
              res += to_string(ID) + "." + i.second + "->" +
                     to_string((int)account[id].milk[i.second]) +
                     unit[i.second] + "\n";
            }
          }
          send_message(e.target, res);
          send_message(e.target, "请选择#+要食用的食物");
          account[id].flag = 5;
        }

        if (account[id].flag == 61) {
          trans_many = read(msg);
          account[id].flag = 0;
          if (trans_many > account[id].money) {
            send_message(e.target, "操作失败!\n失败原因:余额不足");
          } else {
            account[id].money -= trans_many;
            account[trans_to].money += trans_many;
            send_message(e.target, "成功转账!\n当前余额" +
                                       to_string((int)account[id].money) +
                                       "🐱币");
            save(trans_to);
          }
        }

        if (account[id].flag == 6) {
          trans_to = sread(msg);
          send_message(e.target, "请输入#+要转账的金额:");
          account[id].flag = 61;
        }

        if (msg == "#转账") {
          send_message(e.target, "您的账户余额为" +
                                     to_string((int)account[id].money) + "🐱币");
          send_message(e.target, "请输入#+要转账的用户的qq:");
          account[id].flag = 6;
        }

        save(id);
        writeprice();
      }
    } catch (ApiError &) { // 忽略发送失败
    }
    if (e.is_anonymous()) {
      logging::info("群聊", "消息是匿名消息, 匿名昵称: " + e.anonymous.name);
    }
    e.block(); // 阻止当前事件传递到下一个插件
  });

  on_group_upload([](const auto &e) { // 可以使用 auto 自动推断类型
    stringstream ss;
    ss << "您上传了一个文件, 文件名: " << e.file.name
       << ", 大小(字节): " << e.file.size;
    try {
      send_message(e.target, ss.str());
    } catch (ApiError &) {
    }
  });

  on_disable([] { on = false; });
}

CQ_MENU(menu_demo_1) { logging::info("菜单", "点击菜单1"); }

CQ_MENU(menu_demo_2) { send_private_message(10000, "测试"); }

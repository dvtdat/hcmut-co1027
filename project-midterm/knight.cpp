#include "knight.h"

const int inf = 2e9 + 7;
const int maxN = 200010;

int maxHP = 999;
const int maxLevel = 10;
const int maxRemedy = 99;
const int maxMaidenKiss = 99;
const int maxPhoenixDown = 99;

const int minHP = 0;
const int minLevel = 0;
const int minRemedy = 0;
const int minMaidenKiss = 0;
const int minPhoenixDown = 0;

int prefixOdd[110];
bool isPrime[1500], isFibo[1500];
int fibo[30];

void lowercase(string &s)
{
    for (int i = 0; i < s.length(); ++i)
    {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = s[i] - 'A' + 'a';
    }
}

void initialSetUp()
{
    for (int i = 0; i <= 1100; ++i) isPrime[i] = true, isFibo[i] = false;

    int tmp = 99;
    prefixOdd[1] = tmp;
    for (int i = 2; i <= 100; ++i)
    {
        tmp -= 2;
        prefixOdd[i] = prefixOdd[i - 1] + tmp;
    }

    fibo[0] = 1, fibo[1] = 1;
    for (int i = 2; i <= 20; ++i)
    {
        fibo[i] = fibo[i - 1] + fibo[i - 2];
        isFibo[fibo[i]] = true;
    }

    isPrime[1] = false;
    for (int i = 2; i * i <= 1100; ++i)
    {
        if (isPrime[i])
        {
            for (int j = i * i; j <= 1100; j += i) isPrime[j] = false;
        }
    }
}

struct Knights
{
    int healthPoint, level, remedy, maidenKiss, phoenixDown;
    int rescue = 0;

    int prevHealthPoint = 0, prevLevel = 1;
    bool isArthur = false, isLancelot = false;

    void init(string s)
    {
        stringstream ss(s);
        ss >> healthPoint >> level >> remedy;
        ss >> maidenKiss >> phoenixDown;

        maxHP = healthPoint;
        if (isPrime[healthPoint]) isLancelot = true;
        if (healthPoint == 999) isArthur = true;
    }

    void display()
    {
        cout << "HP=" << healthPoint;
        cout << ", level=" << level;
        cout << ", remedy=" << remedy;
        cout << ", maidenkiss=" << maidenKiss;
        cout << ", phoenixdown=" << phoenixDown;
        cout << ", rescue=" << rescue << endl;
    }

    struct Tiny
    {
        bool isTiny = false;
        int duration = 0;
    } tiny;

    struct Frog
    {
        bool isFrog = false;
        int duration = 0;
    } frog;

    void check()
    {
        if (tiny.isTiny)
        {
            if (remedy) 
            {
                tiny.isTiny = false;
                tiny.duration = 0;
                remedy = max(minRemedy, remedy - 1);
                healthPoint = min(healthPoint * 5, maxHP);
            }
            else
            {
                tiny.duration = max(0, tiny.duration - 1);
                if (tiny.duration == 0)
                {
                    tiny.isTiny = false;
                    healthPoint = min(healthPoint * 5, maxHP);
                }
            }
        }

        if (frog.isFrog)
        {
            if (maidenKiss) 
            {
                frog.isFrog = false;
                frog.duration = 0;
                maidenKiss = max(minMaidenKiss, maidenKiss - 1);
                level = prevLevel;
            }
            else
            {
                frog.duration = max(0, frog.duration - 1);
                if (frog.duration == 0)
                {
                    frog.isFrog = false;
                    level = prevLevel;
                }
            }
        }

        if (healthPoint < 0)
        {
            if (phoenixDown > 0)
            {            
                healthPoint = maxHP;
                phoenixDown = max(minPhoenixDown, phoenixDown - 1); return;
            }
            rescue = 0;
        }
    }

} knight;

struct Loots
{
    string lootFile[10];
    int num = 0;

    void init(string s)
    {
        string tmp = "";
        for (int i = 0; i < s.length(); ++i)
        {
            if (s[i] == ',')
            {
                lootFile[num++] = tmp;
                tmp = "";
            }
            else tmp += s[i];
        }

        lootFile[num] = tmp;
    }
} loot;

struct Events
{
    int arr[maxN];
    int num = 0;

    void init(string s)
    {
        stringstream ss(s);
        int intInp;
        while (ss >> intInp) arr[++num] = intInp;
    }

    struct List
    {
        void princessRescued(Knights &knight)
        {
            knight.rescue = 1;
        }

        void meetMonster(Knights &knight, int type, int index)
        {
            int damage[6] = {0, 10, 15, 45, 75, 85};
            int levelO = index > 6 ? (index % 10 > 5 ? index % 10 : 5) : index % 10;

            if (knight.level > levelO || knight.isArthur || knight.isLancelot)
            {
                knight.level = min(maxLevel, knight.level + 1); return;
            }
            if (knight.level == levelO) return;

            knight.healthPoint -= levelO * damage[type];
            knight.check();
        }

        void meetShaman(Knights &knight, int index) 
        {
            if (knight.tiny.isTiny) return;

            int levelO = index > 6 ? (index % 10 > 5 ? index % 10 : 5) : index % 10;
            if (knight.level > levelO  || knight.isArthur || knight.isLancelot)
            {
                knight.level = min(maxLevel, knight.level + 2); return;
            }
            if (knight.level == levelO) return;

            knight.healthPoint = max(knight.healthPoint / 5, 1);
            knight.tiny.isTiny = true;
            knight.tiny.duration = 4;
            knight.check();
        }
        
        void meetSirenVajsh(Knights &knight, int index) 
        {
            if (knight.frog.isFrog) return;

            int levelO = index > 6 ? (index % 10 > 5 ? index % 10 : 5) : index % 10;
            if (knight.level > levelO  || knight.isArthur || knight.isLancelot)
            {
                knight.level = min(maxLevel, knight.level + 2); return;
            }
            if (knight.level == levelO) return;

            knight.prevLevel = knight.level;
            knight.level = 1;
            knight.frog.isFrog = true;
            knight.frog.duration = 4;
            knight.check();
        }

        void pickUpMushMario(Knights &knight)
        {
            int addedHP = prefixOdd[((knight.level + knight.phoenixDown) % 5 + 1) * 3] % 100;
            knight.healthPoint = min(maxHP, knight.healthPoint + addedHP);
        }

        void pickUpMushFibo(Knights &knight)
        {
            if (knight.healthPoint == 1) return;
            int i = knight.healthPoint;
            while (!isFibo[i]) i--;
            knight.healthPoint = i;
        }

        void pickUpMushGhost(Knights &knight)
        {

        }

        void meetBowser(Knights &knight)
        {

        }

        void meetMerlin(Knights &knight)
        {

        }


        void pickUpItem(Knights &knight, int type)
        {
            if (type == 15) knight.remedy = min(maxRemedy, knight.remedy + 1);
            if (type == 16) knight.maidenKiss = min(maxMaidenKiss, knight.maidenKiss + 1);
            if (type == 17) knight.phoenixDown = min(maxPhoenixDown, knight.phoenixDown + 1);

            knight.check();
        }

    } eventList;

    void lookUp(Knights &knight, int index)
    {
        if (arr[index] == 0) eventList.princessRescued(knight);
        if (1 <= arr[index] && arr[index] <= 5) eventList.meetMonster(knight, arr[index], index);
        if (arr[index] == 6) eventList.meetShaman(knight, index);
        if (arr[index] == 7) eventList.meetSirenVajsh(knight, index);
        if (15 <= arr[index] && arr[index] <= 17) eventList.pickUpItem(knight, arr[index]);
        if (arr[index] == 11) eventList.pickUpMushMario(knight);
        if (arr[index] == 12) eventList.pickUpMushFibo(knight);
    }


} events;

void dataInput(string file_input, Knights &knight, Events &events)
{
    ifstream fileIn;
    fileIn.open(file_input);

    string firstInpStr, secondInpStr, thirdInpStr;
    getline(fileIn, firstInpStr);
    getline(fileIn, secondInpStr);
    getline(fileIn, thirdInpStr);

    knight.init(firstInpStr);
    events.init(secondInpStr);
    loot.init(thirdInpStr);
}

void adventureToKoopa(string file_input, int &healthPoint, int &level, int &remedy, int &maidenKiss, int &phoenixDown, int &rescue) 
{
    initialSetUp();
    dataInput(file_input, knight, events);

    int i = 0;
    while (i <= events.num && knight.rescue == 0 && knight.healthPoint >= 0)
    {
        knight.display();
        events.lookUp(knight, ++i);
    }
    if (i == events.num && knight.healthPoint >= 0) knight.rescue = 1;

    knight.display();
}
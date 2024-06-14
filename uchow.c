#include "uchow.h"

void story(int32_t choice)
{
    print_out();

    if(choice == 0)
    {
        
        caption = "Start the game!\n";
        
        Option options[] =
        {
            {"(1)去警局尋找當地警員協助", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 1},
            {"(2)到熱鬧的大街上看看", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 14},
            {"(3)肚子餓了先去吃飯", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 3}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[3].options = options;
        scenes[0].dialogs[3].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 1)
    {
        Option options[] =
        {
            {"(1)你好，我是一個新手記者，剛入職這個行業三個月，希望我們合作愉快。", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 2},
            {"(2)你好，我的姊姊是這起事件的受害者之一，所以請你一定要把所有資訊都告 訴我，我會盡所能協助破案。", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 15}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 2)
    {
        caption = "警長給了你四名受害者的個人資料。\n";
        /*
        printf("獲得道具:受害者資料(圖)\n");
        printf("    小美，女性，30歲，小學老師，長捲髮\n");
        printf("    小麗，女性，18歲，學生，長捲髮\n");
        printf("    小漂，女性，32歲，家庭主婦，長捲髮\n");
        printf("    小亮，女性，23歲，服務員，長捲髮\n");
        
            printf("警長好感+2\n");
            like.Police += 2;
        */
        if(Becca == 1)
        {
            caption = "你發現受害者都是長捲髮，不曉得是不是巧合，此刻你的腦海莫名出現 Becca的臉。\n這時有一個警員火急火燎的衝進辦公室，大喊連環殺人魔又下手了，而受害 者的照片赫然是Becca!\n你決定:";

            Option options[] =
            {
                {"(1)馬上告訴警長自己的猜:連續殺人犯很有可能是對長捲髮有執念，且昨天載Becca回去的男朋友很可疑!", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 17},
                {"(2)趕快離開並偷偷去找東哥請求他的幫助", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 18}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = Option.nextSceneIndex;
        }
        else
        {
            caption = "你發現受害者都是長捲髮，不曉得是不是巧合。\n接下來你決定...";

            Option options[] =
            {
                {"(1)肚子餓了先去吃飯", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 3},
                {"(2)去受害者的家中拜訪", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 11}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = Option.nextSceneIndex;
        }
    }
    else if(choice == 3)
    {
        caption = "去了一間麵店，發現旁邊有一個流浪漢蹲坐在路邊，看著前面雙眼無神。\n你決定...";

        Option options[] =
        {
            {"(1)Hi!請問你可以幫我吃小菜嗎，不小心點太多了!", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 4},
            {"(2)先生你很餓吧?我請你吃飯吧!", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 9},
            {"(3)不理他", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 10}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[3].options = options;
        scenes[0].dialogs[3].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 4)
    {
        caption = "流浪漢感激地看了你一眼，開始狼吞虎嚥，這時你才發現它少了一條右臂， 你決定...";
        /*
            printf("流浪看好感+3\n");
            like.Street += 3;
        */
        Option options[] =
        {
            {"(1)裝沒看到，跟他閒話家常", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 5},
            {"(2)問他的右臂發生了什麼事", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 7}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 5)
    {
        caption = "他說他叫東哥，原本在鎮上開一間水果店，經歷了一些意外從此消沉。你聽到後沒說什麼，給了他一些零錢後讓他有事可以找你便離開了。\n";
        if(Becca)
        {
            /*
                printf("東哥好感+2\n");
                printf("Becca好感+1\n");
                like.Don += 2;
                like.Becca += 1;
                printf("離開之前，東哥偷偷遞給你一張紙條，上面寫著:有事第一個找我。\n");
                printf("獲得道具:東哥的紙條\n");
                backpack.donpaper +=1;
            */
            caption = "回租屋處你累的直接睡了，到了半夜，迷糊中被電話吵醒，看見來電顯示竟然是Becca，你決定...";
            Option options[] =
            {
                {"(1)超想睡，直接把電話掛了", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 19},
                {"(2)接起來", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 20}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = Option.nextSceneIndex;
        }
        else
        {
            /*
            printf("東哥好感+2\n");
            like.Don += 2;
            */
            caption = "很快地結束飯局，你回到租屋處整理資料。";

            choice = 8;
            Option.nextSceneIndex = 8;
            return;
        }
    }
    else if(choice == 6)
    {
        caption = "警長表示兇手將受害者的頭皮割開，為了不對她們造成陰影才不讓他們看。你表示理解，同時也對殺人犯的手段表示不寒而慄。\n線索就停在了這裡，你發現推理真的沒有想像簡單。\n看來你沒有當偵探的潛力......\nGAME OVER...\n";
        return;
    }
    else if(choice == 7)
    {
        caption = "他看了你一眼，輕描淡寫的帶過了。";
        /*
        printf("流浪漢好感-1\n");
        like.Street -= 1;
        */
        caption = "很快地結束飯局，你回到租屋處整理資料。";
        choice = 8;
        Option.nextSceneIndex = 8;
        return;
    }
    else if(choice == 8)
    {
        caption = "你發現什麼都整理不出來，推理好難。\n看來你沒有當偵探的潛力......\nGAME OVER...";
        return;
    }
    else if(choice == 9)
    {
        caption = "他開始狼吞虎嚥，這時你才發現它少了一條右臂，你決定..\n";
        /*
            printf("流浪看好感+2\n");
            like.Street += 2;
        */

        Option options[] =
        {
            {"(1)裝沒看到，跟他閒話家常", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 5},
            {"(2)問他的右臂發生了什麼事", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 7}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 10)
    {
        end = 1;
        if(Becca)
        {
            /*
                printf("Becca好感+1\n");
                like.Becca += 1;
            */
            caption = "很快地結束飯局，Becca的男友來接他離開，秉持著交朋友的態度，你們互 打電話加了號碼。\n回租屋處你累的直接睡了，到了半夜，迷糊中被電話吵醒，看見來電顯示竟然是Becca，你決定...";
            Option options[] =
            {
                {"(1)超想睡，直接把電話掛了", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 19},
                {"(2)接起來", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 20}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = Option.nextSceneIndex;
        }
        else
        {
            caption = "看來你沒有當偵探的潛力......\nGAME OVER...";
            return;
        }
    }
    else if(choice == 11)
    {
        caption = "你來到小美的家，她的家人悲慟的表示她生前並沒有跟任何人結仇，但她的死狀卻極其悽慘，甚至警察不讓他們親眼見最後一面，而剩下三家也是一樣的狀況。\n你十分疑惑，決定...";
        Option options[] =
        {
            {"(1)回警局詢問警長", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 6},
            {"(2)來到殯儀館尋找死者的法醫", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 12}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 12)
    {
        caption = "法醫表示兇手將死者的頭皮割開，手段非常殘忍，而且兇手將整片頭皮連同頭髮一起帶走，每位受害者都是如此，這令他感到十分不解。\n你發現了「頭髮」這個關鍵字，會不會這對兇手有特別的意義...?\n你決定...";
        Option options[] =
        {
            {"(1)回警局向警長報告這個發現", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 13},
            {"(2)到大街上找別的線索", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 14}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 13)
    {
        caption = "警長若有所思地看了你一眼，然後告訴你這個案子有其他專人處理，不方便 你插手了。\n看來你沒有當偵探的潛力......\nGAME OVER...");
        return;
    }
    else if(choice == 14)
    {
        caption = "發現路上有很多學生，看一下時間才發現剛好放學了，你覺得進一所附近的高中看看。";
        Option options[] =
        {
            {"(1)去生態池看看", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 16},
            {"(2)去教學區看", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 21},
            {"(3)去體育館看", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 26}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[3].options = options;
        scenes[0].dialogs[3].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 15)
    {
        if(Becca)
        {
            caption = "看來你沒有當偵探的潛力......\nGAME OVER...";
            return;
        }
        else
        {
            /*
                printf("警長給了你沒有照片的個人資料。\n");
                printf("    小美，女性，30歲，小學老師，長捲髮\n");
                printf("    小麗，女性，18歲，學生，長捲髮\n");
                printf("    小漂，女性，32歲，家庭主婦，長捲髮\n");
                printf("    小亮，女性，23歲，服務員，長捲髮\n");
                backpack.nopicture += 1;
                printf("警長好感+1");
                like.Police += 1;
            }
            */
            caption = "你沒有從中得到任何發現......\n接下來你決定...";

            Option options[] =
            {
                {"(1)肚子餓了先去吃飯", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 3},
                {"(2)去受害者的家中拜訪", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 11}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = Option.nextSceneIndex;
        }
    }
    else if(choice == 16)
    {
        caption = "你來到生態池，剛好看到一個女孩失足掉進水裡，沒有過多猶豫，你直接上前救援。\n為了感謝你，女孩堅持請你吃飯。";
        /*
            printf("女孩好感+3\n");
            like.girl += 3;
        */

        caption = "女孩叫做Becca，你們去了一間麵店，發現旁邊有一個流浪漢蹲坐在路邊，看著前面雙眼無神。";

        Becca = 1;
        Option options[] =
        {
            {"(1)Hi!請問你可以幫我吃小菜嗎，不小心點太多了!", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 4},
            {"(2)先生你很餓吧?我請你吃飯吧!", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 9},
            {"(3)不理他", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 10}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[3].options = options;
        scenes[0].dialogs[3].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 17)
    {
        caption = "警長看了你一眼，告訴你接下來是他們的辦公時間，請你離開。\n看來你沒有當偵探的潛力......\nGAME OVER...");
        return;
    }
    else if(choice == 18)
    {
        if(like.Don >= 5)
        {
            caption = "東哥沒多想直接告訴你，其實他的手是被他的好兄弟Kevin砍斷的，因為他撞見了Kevin的殺人現場，逃跑過程被砍斷了一隻手，這些年為了不要被認出，他一直裝做乞丐。然而昨天他發現Becca的男友竟就是Kevin!\n他說Kevin在母親過世後開始有瘋魔的狀態，只要看見長捲髮都會認為母親還在，因此開始不斷找長捲髮的女性下手。\n你很疑惑為什麼東哥不向上舉報，東哥只說了句:Kevin的父親就市警局最德高望重的警長，他會替他掩蓋好一切罪刑並保護他。\n你在此終於得知了真相，但要怎麼對抗警局的勢力又是另外一個故事了。\nYOU WIN!!!";
            return;
        }
        else
        {
            caption = "東哥表示Becca只是一個沒見過的女孩子，他幫不上什麼忙。\n看來你沒有當偵探的潛力......\nGAME OVER...";
            return;
        }
    }
    else if(choice == 19)
    {
        if(end)
        {
            caption = "一覺到天亮，隔天到警局想尋求警察協助，卻意外得知Becca竟成了連環殺人犯的第五名受害者，你不免想起了作天晚上的那通電話，悲痛之下你決定先暫時擱下這次任務調整心情。\n看來你沒有當偵探的潛力......\nGAME OVER...";
            return;
        }
        else
        {
            if(line1 == 1)
            {
                caption = "接下來你決定來到警局尋求當地警察的幫助。\n這時有一個警員火急火燎的衝進辦公室，大喊連環殺人魔又下手了，而受害者的照片赫然是Becca!";
                Option options[] =
                {
                    {"(1)馬上告訴警長自己的猜:連續殺人犯很有可能是對長捲髮有執念，且昨天載Becca回去的男朋友很可疑!", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 17},
                    {"(2)趕快離開並偷偷去找東哥請求他的幫助", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 18}
                };
                int totalOptions = sizeof(options) / sizeof(options[0]);

                scenes[0].dialogs[2].options = options;
                scenes[0].dialogs[2].totalOptions = totalOptions;

                choice = Option.nextSceneIndex;
            }
            else
            {
                caption = "接下來你決定來到警局尋求當地警察的幫助。當地的警察聽到你要調查這起案件非常開心，因為這已經讓他們頭痛很久了，經過討論，他們決定讓警局裡德高望重的警長協助你調查。\n對警長的自我介紹:";
                Option options[] =
                {
                    {"(1)你好，我是一個新手記者，剛入職這個行業三個月，希望我們合作愉快。", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 2},
                    {"(2)你好，我的姊姊是這起事件的受害者之一，所以請你一定要把所有資訊都告訴我，我定會盡所能協助破案。", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 15}
                };
                int totalOptions = sizeof(options) / sizeof(options[0]);

                scenes[0].dialogs[2].options = options;
                scenes[0].dialogs[2].totalOptions = totalOptions;

                choice = Option.nextSceneIndex;
            }
        }
    }
    else if(choice == 20)
    {
        caption = "你只聽到Becca故作冷靜地說:你帶我來和平公園做什麼.....電話馬上被掛斷，你頓感不妙，馬上驅車前往和平公園。\n來到公園，你一邊喊著Becca一邊往深處走，這時你踩到一個軟綿綿的東西，低頭一看竟然是倒在血泊中的Becca!\n你還來不及大喊，馬上感覺到脖子上有冰涼的觸感，低頭一看竟是一把刀，耳邊傳來Becca男友的聲音:被你發現了.....\n隨後你的脖子被一抹，Becca男友隨後自殺，你的任務就此失敗。\nGAME OVER...";
        return;
    }
    else if(choice == 21)
    {
        caption = "來到教學區，你發現因為現在下課了，附近空無一人，但地上有一副遺落的藍芽耳機，打開後發現他的名稱是Becca's。";
        /*
            printf("獲得道具:Becca的藍芽耳機");
            backpack.airpods += 1;
        */
        caption = "撿起來後你決定...";

        Option options[] =
        {
            {"(1)發上學校尋物平台", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 22},
            {"(2)先收著", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 25}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 22)
    {
        caption = "果然有人來聯絡你，一個叫Becca的女生跟你約在校門口見面。\nBecca表達了對你的感謝，你們也剛好聊了一下。";
        /*
            printf("Becca好感+2");
            like.Becca += 2;
        */

        Option options[] =
        {
            {"(1)最近關於連續殺人嫌犯，你有聽到什麼消息嗎?", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 23},
            {"(2)你的名字很好聽", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 24}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 23)
    {
        caption = "Becca表示有聽說但沒頭緒。為了感謝你他決定請你吃飯。";
        choice = 3;
        Option.nextSceneIndex = 3;
        return;
    }
    else if(choice == 24)
    {
        caption = "這不是把妹時間，請公事公辦。\n看來你沒有當偵探的潛力......\nGAME OVER...");
        return;
    }
    else if(choice == 25)
    {
        caption = "肚子餓了，你決定去吃飯。";
        choice = 3;
        Option.nextSceneIndex = 3;
        return;
    }
    else if(choice == 26)
    {
        caption = "現在正好是女籃的比賽時間，其中一個短髮女孩引起了你的注意，他自信張揚，籃球在他手中劃過完美的三分弧線。";

        Option options[] =
        {
            {"(1)上前搭話", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 27},
            {"(2)在旁邊看", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 32}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 27)
    {
        Option options[] =
        {
            {"(1)嗨!可以跟你切磋一下嗎?", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 28},
            {"(2)你猜我什麼星座?是為你量身訂做🫰", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 31}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 28)
    {
        caption = "在打球過程中你知道她叫Jenny，原本留著一頭長髮，後來因為投入熱愛的籃球而剪去，前男友卻為此跟她分手。";

        Option options[] =
        {
            {"(1)太誇張了吧!他到底是喜歡你還是喜歡你的頭髮??", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 29},
            {"(2)其實我覺得他這樣也有道理", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 30}
        };
        int totalOptions = sizeof(options) / sizeof(options[0]);

        scenes[0].dialogs[2].options = options;
        scenes[0].dialogs[2].totalOptions = totalOptions;

        choice = Option.nextSceneIndex;
    }
    else if(choice == 29)
    {
        caption = "Jenny神秘兮兮地湊上來，說前男友還真的可能是喜歡他的頭髮，因為在他們分手後他又很快地交了新女友，正是他的閨密Becca，而Becca也是留一頭長捲髮。\n這時候Becca來給Jenny送水了，你們相談甚歡，決定找一間餐廳吃飯。";
        choice = 3;
        Option.nextSceneIndex = 3;
        return;
    }
    else if(choice == 30)
    {
        caption = "Jenny大傻眼，直接帶著她的球離開了\n看來你沒有當偵探的潛力......\nGAME OVER...";
        return;
    }
    else if(choice == 31)
    {
        caption = "她嫌惡的看了你一眼，直接轉頭離開\nGAME OVER...";
        return;
    }
    else if(choice == 32)
    {
        caption = "看來你沒有當偵探的潛力......\nGAME OVER...";
        return;
    }
    else if(choice == 33)
    {
        choice = 3;
        Option.nextSceneIndex = 3;
    }
}

void END()
{
    caption = "遊戲中斷...";
    return;
}

void print_out()
{
    if(backpack.airpods > 1 || backpack.donpaper > 1 || backpack.nopicture > 1 || backpack.picture > 1)
    {
        printf("-----------------------------------\n");
        printf("My backpack: \n");
    
        if(backpack.airpods > 1)
        {
            printf("    You have Becca的藍芽耳機\n");
        }
        if(backpack.donpaper > 1)
        {
            printf("    You have 東哥的紙條\n");
        }
        if(backpack.nopicture > 1)
        {
            printf("    You have 沒照片受害者資料(圖)\n");
        }
        if(backpack.picture > 1)
        {
            printf("    You have 受害者資料(圖)\n");
        }
        printf("-----------------------------------\n");
    }
    if(like.Becca > 1 || like.Don > 1 || like.girl > 1 || like.Police > 1 || like.Street > 1)
    {
        printf("-----------------------------------\n");
        printf("Who like me: \n");
    
        if(like.Becca > 1)
        {
            printf("    Becca like me.(%d)\n", like.Becca);
            like.Becca += like.girl;
            like.girl= 0; 
        }
        if(like.Don > 1)
        {
            printf("    東哥 like me.(%d)\n", like.Don);
            like.Don += like.Street;
            like.Street = 0; 
        }
        if(like.girl > 1)
        {
            printf("    女孩 like me.(%d)\n", like.girl);
        }
        if(like.Police > 1)
        {
            printf("    警長 like me.(%d)\n", like.Police);
        }
        if(like.Street)
        {
            printf("    流浪漢 like me.(%d)\n", like.Street);
        }
        printf("-----------------------------------\n");
    }
}
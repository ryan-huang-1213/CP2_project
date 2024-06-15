#include "uchow.h"

void load_story(Scene scenes[]) {
    int story=0;
    // scene[0]
    // 為 backgrounds 動態分配記憶體
    scenes[0].backgrounds = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[0].backgrounds != NULL) {
        scenes[0].backgrounds[0] = (Asset){"source/image/maldives.bmp", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[0].backgrounds[1] = (Asset){"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[0].totalBackgrounds = 2;
    }

    // 為 characters 動態分配記憶體
    scenes[0].characters = NULL;
    scenes[0].totalCharacters = 0;

    // 為 dialogs 動態分配記憶體
    scenes[0].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[0].dialogs != NULL) {
        scenes[0].dialogs[0] = (Dialog){"遊戲開始 ! \n你想要先去哪裡逛逛呢 ? ", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[0].totalDialogs = 1;
    }
    scenes[0].currentDialogIndex = 0;
    scenes[0].currentBackgroundIndex = 0;

    scenes[0].options = (Option*)calloc(3 , sizeof(Option));
    if(scenes[0].options != NULL){
        scenes[0].options[0] = (Option){"(1)去警局尋找當地警員協助",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},1};
        scenes[0].options[1] = (Option){"(2)到熱鬧的大街上看看",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},14};
        scenes[0].options[2] = (Option){"(3)肚子餓了先去吃飯",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},3};
    }
    scenes[0].dialogs[0].options = scenes[0].options;
    scenes[0].dialogs[0].totalOptions = 3;

    // scene[1]
    // 為 backgrounds 動態分配記憶體
    scenes[1].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[1].backgrounds != NULL) {
        scenes[1].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[1].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[1].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[1].characters != NULL) {
        scenes[1].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[1].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[1].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[1].dialogs != NULL) {
        scenes[1].dialogs[0] = (Dialog){"那裡看起來有一個警員可以幫忙，去問問看吧", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[1].characters, 1, NULL, 0, NULL};
        scenes[1].totalDialogs = 1;
    }
    scenes[1].currentDialogIndex = 0;
    scenes[1].currentBackgroundIndex = 0;

    scenes[1].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[1].options != NULL){
        scenes[1].options[0] = (Option){"(1)你好，我是一個新手記者，剛入職這個行業三個月，希望我們合作愉快。",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 150, 400, 50},3};
        scenes[1].options[1] = (Option){"(2)你好，我的姊姊是這起事件的受害者之一，所以請你一定要把所有資訊都告訴我，我會盡所能協助破案。",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},15};
    }
    scenes[1].dialogs[0].options = scenes[1].options;
    scenes[1].dialogs[0].totalOptions = 2;

    // scene[2]
    // 為 backgrounds 動態分配記憶體
    scenes[2].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[2].backgrounds != NULL) {
        scenes[2].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[2].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[2].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[2].characters != NULL) {
        scenes[2].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[2].characters[1] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[2].totalCharacters = 2;
    }

    // 為 dialogs 動態分配記憶體
    scenes[2].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[2].dialogs != NULL) {
        scenes[2].dialogs[0] = (Dialog){"警長給了你四名受害者的個人資料。\n獲得道具:受害者資料", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[2].dialogs[1] = (Dialog){"你發現受害者都是長捲髮，不曉得是不是巧合。\n接下來，你打算", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[2].totalDialogs = 2;
    }
    scenes[2].currentDialogIndex = 0;
    scenes[2].currentBackgroundIndex = 0;

    scenes[2].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[2].options != NULL){
        scenes[2].options[0] = (Option){"(1)肚子餓了先去吃飯",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},3};
        scenes[2].options[1] = (Option){"(2)去受害者的家中拜訪",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},11};
    }
    scenes[2].dialogs[1].options = scenes[2].options;
    scenes[2].dialogs[1].totalOptions = 2;

    // scene[3]
    // 為 backgrounds 動態分配記憶體
    scenes[3].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[3].backgrounds != NULL) {
        scenes[3].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[3].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[3].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[3].characters != NULL) {
        scenes[3].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[3].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[3].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[3].dialogs != NULL) {
        scenes[3].dialogs[0] = (Dialog){"去了一間麵店，發現旁邊有一個流浪漢蹲坐在路邊，看著前面雙眼無神。\n你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[3].characters, 1, NULL, 0, NULL};
        scenes[3].totalDialogs = 1;
    }
    scenes[3].currentDialogIndex = 0;
    scenes[3].currentBackgroundIndex = 0;

    scenes[3].options = (Option*)calloc(3 , sizeof(Option));
    if(scenes[3].options != NULL){
        scenes[3].options[0] = (Option){"(1)Hi!請問你可以幫我吃小菜嗎，不小心點太多了!",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},4};
        scenes[3].options[1] = (Option){"(2)先生你很餓吧?我請你吃飯吧!",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},9};
        scenes[3].options[2] = (Option){"(3)不理他",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},10};
    }
    scenes[3].dialogs[0].options = scenes[3].options;
    scenes[3].dialogs[0].totalOptions = 3;

    // scene[4]
    story = 4;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"流浪漢感激地看了你一眼，開始狼吞虎嚥，這時你才發現它少了一條右臂\n你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;

    scenes[story].options = (Option*)calloc(3 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)裝沒看到，跟他閒話家常",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},5};
        scenes[story].options[1] = (Option){"(2)問他的右臂發生了什麼事",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},7};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;


    // scene[5]
    story = 5;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他說他叫東哥，原本在鎮上開一間水果店，經歷了一些意外從此消沉。你聽到後沒說什麼，給了他一些零錢後讓他有事可以找你便離開了。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].dialogs[1] = (Dialog){"很快地結束飯局，你回到租屋處準備資料", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 2;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 8;

    // scene[6]
    story = 6;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"警長表示兇手將受害者的頭皮割開，為了不對她們造成陰影才不讓他們看。你表示理解，同時也對殺人犯的手段表示不寒而慄。\n線索就停在了這裡，你發現推理真的沒有想像簡單。\n看來你沒有當偵探的潛力......\nGAME OVER...\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 0;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[7]
    // 為 backgrounds 動態分配記憶體
    story = 7;
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他看了你一眼，輕描淡寫的帶過了。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].dialogs[1] = (Dialog){"很快地結束飯局，你回到租屋處整理資料。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 2;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 8;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[8]
    story = 8;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/pure_black.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = NULL;
    scenes[story].totalCharacters = 0;

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你發現什麼都整理不出來，推理好難。\n看來你沒有當偵探的潛力......\nGAME OVER...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[9]
    story = 9;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他開始狼吞虎嚥，這時你才發現它少了一條右臂，你決定..\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 10;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)裝沒看到，跟他閒話家常",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},5};
        scenes[story].options[1] = (Option){"(2)問他的右臂發生了什麼事",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},7};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[10]
    story = 10;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他開始看來你沒有當偵探的潛力......\nGAME OVER...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 10;

    scenes[story].options = NULL;
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[11]
    story = 11;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/受害者的家裡.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/受害者的家人.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你來到小美的家，她的家人悲慟的表示她生前並沒有跟任何人結仇，但她的死狀卻極其悽慘，甚至警察不讓他們親眼見最後一面，而剩下三家也是一樣的狀況。..\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 11;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)回警局詢問警長",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},6};
        scenes[story].options[1] = (Option){"(2)來到殯儀館尋找死者的法醫",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},12};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[15]
    // 為 backgrounds 動態分配記憶體
    scenes[15].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[15].backgrounds != NULL) {
        scenes[15].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[15].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[15].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[15].characters != NULL) {
        scenes[15].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[15].characters[1] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[15].totalCharacters = 2;
    }

    // 為 dialogs 動態分配記憶體
    scenes[15].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[15].dialogs != NULL) {
        scenes[15].dialogs[0] = (Dialog){"你沒有從中得到任何發現......\n接下來你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[15].totalDialogs = 1;
    }
    scenes[15].currentDialogIndex = 0;
    scenes[15].currentBackgroundIndex = 0;

    scenes[15].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[15].options != NULL){
        scenes[15].options[0] = (Option){"(1)肚子餓了先去吃飯",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},3};
        scenes[15].options[1] = (Option){"(2)去受害者的家中拜訪",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},11};
    }
    scenes[15].dialogs[0].options = scenes[15].options;
    scenes[15].dialogs[0].totalOptions = 2;


}

    /*
    // 為 backgrounds 動態分配記憶體
    scenes[0].backgrounds = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[0].backgrounds != NULL) {
        scenes[0].backgrounds[0] = (Asset){"source/image/maldives.bmp", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[0].backgrounds[1] = (Asset){"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[0].totalBackgrounds = 2;
    }

    // 為 characters 動態分配記憶體
    scenes[0].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[0].characters != NULL) {
        scenes[0].characters[0] = (Asset){"source/character/LG.png", NULL, {0, 0, 250, 400}};
        scenes[0].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[0].dialogs = (Dialog*)calloc(4 , sizeof(Dialog));
    if (scenes[0].dialogs != NULL) {
        scenes[0].dialogs[0] = (Dialog){"這是馬爾地夫1，是一個眾所周知的旅遊勝地", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        printf("flag \n");
        scenes[0].dialogs[1] = (Dialog){"沙灘很美，海很棒", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[0].characters, 1, NULL, 0, NULL};
        printf("flag \n");
        scenes[0].dialogs[2] = (Dialog){"但 LG 沒有足夠的錢錢 因此他需要去找一個額外的工作來賺更多錢錢，他要做甚麼工作呢 ? ", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1, NULL, 0, NULL, 0, NULL};
        scenes[0].dialogs[3] = (Dialog){"LG 準備要去工作了", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1, NULL, 0, NULL, 0, NULL};
        scenes[0].totalDialogs = 4;
    }
    scenes[0].currentDialogIndex = 0;
    scenes[0].currentBackgroundIndex = 0;

    scenes[0].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[0].options != NULL){
        scenes[0].options[0] = (Option){"去微軟工作",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},0};
        scenes[0].options[1] = (Option){"去端墨西哥粽",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},0};
    }
    scenes[0].dialogs[3].options = scenes[0].options;
    scenes[0].dialogs[3].totalOptions = 2;
    */
    
    // ... 初始化其他場景 ...

    // 記得在不再需要時釋放記憶體

/*
void load_story(Scene scenes[]){
    // 初始化第一個場景
    scenes[0].backgrounds = (Asset[2]){
        {"source/image/maldives.bmp", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}},
        {"source/image/tmp_background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}
    };
    scenes[0].totalBackgrounds = 2;
    scenes[0].characters = (Asset[1]){{"source/character/LG.png", NULL, {0, 0, 0, 0}}};
    scenes[0].totalCharacters = 1;
    scenes[0].dialogs = (Dialog[4]){
        {"這是馬爾地夫，是一個眾所周知的旅遊勝地", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
        {"該地以其優美的大海還有沙灘聞名於世", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0},
        {
            "L紀的夢想是有朝一日可以去馬爾地夫好好度假。奈何教授薪資對於遊玩馬爾地夫來說，屬實是杯水車薪。這裡臨時增加很多很多很多很多很多的文字",
            NULL,
            {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
            1,
            scenes[0].characters,
            1
        },
        {
            "所以，他決定臨時找一些工作去賺更多錢，好讓夢想早日成真。不過L紀要做甚麼工作呢 ? ",
            NULL,
            {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT},
            1,
            scenes[0].characters,
            1
        }
    };
    scenes[0].totalDialogs = 4;
    scenes[0].currentDialogIndex = 0;
    scenes[0].currentBackgroundIndex = 0;
    scenes[0].nextSceneIndex = 1;

    // 初始化第二個場景
    // ... 類似上面的代碼 ...

    // 初始化第三個場景
    // ... 類似上面的代碼 ...

    // 設置角色的位置
    scenes[0].characters[0].rect.x = 0;
    scenes[0].characters[0].rect.y = WINDOW_HEIGHT * 0.3;
    scenes[0].characters[0].rect.w = 250;
    scenes[0].characters[0].rect.h = 450;

    // 初始化選項
    Option options[] = {
        {"去微軟工作", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 2},
        {"掃操場", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 1}
    };
    int totalOptions = sizeof(options) / sizeof(options[0]);

    // 將選項與對話關聯
    scenes[0].dialogs[2].options = options;
    scenes[0].dialogs[2].totalOptions = totalOptions;
}
*/
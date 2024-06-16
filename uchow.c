#include "uchow.h"

void loadFromFile(const char *filename, Scene scenes[], int *totalScenes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open story data file.\n");
        return;
    }
    char buffer[1024];

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "Failed to read total scenes.\n");
        fclose(file);
        return;
    }
    sscanf(buffer, "%d", totalScenes);
    printf("%d\n", *totalScenes);

    for (int i = 0; i < *totalScenes; ++i) {
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            fprintf(stderr, "Failed to read scene keyword.\n");
            fclose(file);
            return;
        }
        printf("%s", buffer);

        int totalBackgrounds = 1, totalDialogs = 0;

        scenes[i].backgrounds = malloc(sizeof(Asset) * totalBackgrounds);
        scenes[i].totalBackgrounds = totalBackgrounds;
        for (int j = 0; j < totalBackgrounds; ++j) {
            scenes[i].backgrounds[j].path = malloc(256);
            if (fgets(scenes[i].backgrounds[j].path, 256, file) == NULL) {
                fprintf(stderr, "Failed to read background path.\n");
                fclose(file);
                return;
            }
            scenes[i].backgrounds[j].path[strcspn(scenes[i].backgrounds[j].path, "\n")] = '\0'; // Remove trailing newline
            printf("%s\n", scenes[i].backgrounds[j].path);

            scenes[i].backgrounds[j].texture = NULL; // Load texture later
        }

        if (fgets(buffer, sizeof(buffer), file) == NULL || sscanf(buffer, "%d", &totalDialogs) != 1) {
            fprintf(stderr, "Failed to read total dialogs.\n");
            fclose(file);
            return;
        }
        printf("%d\n", totalDialogs);

        scenes[i].dialogs = malloc(sizeof(Dialog) * totalDialogs);
        scenes[i].totalDialogs = totalDialogs;
        
        for (int j = 0; j < totalDialogs; ++j) {
            if (fgets(buffer, sizeof(buffer), file) == NULL) {
                fprintf(stderr, "Failed to read dialog keyword.\n");
                fclose(file);
                return;
            }
            printf("%s", buffer);

            scenes[i].dialogs[j].text = malloc(1024);
            if (fgets(scenes[i].dialogs[j].text, 1024, file) == NULL) {
                fprintf(stderr, "Failed to read dialog text.\n");
                fclose(file);
                return;
            }
            scenes[i].dialogs[j].text[strcspn(scenes[i].dialogs[j].text, "\n")] = '\0'; // Remove trailing newline
            
            printf("%s\n", scenes[i].dialogs[j].text);

            int totalDialogCharacters = 1;
            scenes[i].dialogs[j].characters = malloc(sizeof(Asset) * totalDialogCharacters);
            scenes[i].dialogs[j].totalCharacters = totalDialogCharacters;
            for (int k = 0; k < totalDialogCharacters; ++k) {
                if (fgets(buffer, sizeof(buffer), file) == NULL) {
                    fprintf(stderr, "Failed to read character name.\n");
                    fclose(file);
                    return;
                }
                buffer[strcspn(buffer, "\n")] = '\0';
                printf("%s\n", buffer);
            //renew
            scenes[i].dialogs[j].at_background = 0;
            scenes[i].dialogs[j].characters[k].path = malloc(256);
            if (scenes[i].dialogs[j].characters[k].path == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                fclose(file);
                return;
            }
            strcpy(scenes[i].dialogs[j].characters[k].path, buffer);

            //renew
                scenes[i].dialogs[j].characters[k].path = malloc(256);
                if (fgets(scenes[i].dialogs[j].characters[k].path, 256, file) == NULL) {
                    fprintf(stderr, "Failed to read character path.\n");
                    fclose(file);
                    return;
                }
                scenes[i].dialogs[j].characters[k].path[strcspn(scenes[i].dialogs[j].characters[k].path, "\n")] = '\0'; // Remove trailing newline
                printf("%s\n", scenes[i].dialogs[j].characters[k].path);

                scenes[i].dialogs[j].characters[k].texture = NULL; // Load texture later
            }
            if (fgets(buffer, sizeof(buffer), file) == NULL || sscanf(buffer, "%d", &scenes[i].dialogs[j].totalOptions) != 1) {
                fprintf(stderr, "Failed to read total options.\n");
                fclose(file);
                return;
            }

            scenes[i].dialogs[j].options = malloc(sizeof(Option) * scenes[i].dialogs[j].totalOptions);
            scenes[i].options = (Option*)calloc(scenes[i].dialogs[j].totalOptions , sizeof(Option));
            printf("Total: %d\n", scenes[i].dialogs[j].totalOptions);
            for (int k = 0; k < scenes[i].dialogs[j].totalOptions; ++k) {
                scenes[i].dialogs[j].options[k].text = malloc(256);
                if (fgets(scenes[i].dialogs[j].options[k].text, 256, file) == NULL) {
                    fprintf(stderr, "Failed to read option text.\n");
                    fclose(file);
                    return;
                }
                scenes[i].dialogs[j].options[k].text[strcspn(scenes[i].dialogs[j].options[k].text, "\n")] = '\0'; // Remove trailing newline
                printf("%s\n", scenes[i].dialogs[j].options[k].text);

                scenes[i].dialogs[j].options[k].texture = NULL; // Load texture later

                if (fgets(buffer, sizeof(buffer), file) == NULL || sscanf(buffer, "%d", &scenes[i].dialogs[j].options[k].nextSceneIndex) != 1) {
                    fprintf(stderr, "Failed to read option next scene index.\n");
                    fclose(file);
                    return;
                }
                printf("Next: %d\n", scenes[i].dialogs[j].options[k].nextSceneIndex);
                if(scenes[i].options != NULL ){
                scenes[i].options[k] = (Option){scenes[i].dialogs[j].options[k].text,NULL,{WINDOW_WIDTH-400, (WINDOW_HEIGHT - DIALOG_HEIGHT) * ( (k+0.99) / 4.0 ), 400, 50}, scenes[i].dialogs[j].options[k].nextSceneIndex};
                }
            }
            scenes[i].dialogs[0].options = scenes[i].options;
        }
    }
    fclose(file);
}
void load_story(Scene scenes[]) {
    printf("start define \n");
    int story=0;
    // scene[0]
    // 為 backgrounds 動態分配記憶體
    scenes[0].backgrounds = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[0].backgrounds != NULL) {
        scenes[0].backgrounds[0] = (Asset){"source/image/background.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT * 1.33 }};
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
        scenes[1].options[0] = (Option){"(1)你好，我是一個新手記者，剛入職這個行業三個月，希望我們合作愉快。",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 180, 400, 50},2};
        scenes[1].options[1] = (Option){"(2)你好，我的姊姊是這起事件的受害者之一，所以請你一定要把所有資訊都告訴我，我會盡所能協助破案。",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},36};
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
        scenes[2].dialogs[0] = (Dialog){"警長給了你四名受害者的個人資料。\n獲得道具:受害者資料 警長好感 2   <likes Police 2><backpack picture 1>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[2].dialogs[1] = (Dialog){"你發現受害者都是長捲髮，不曉得是不是巧合。\n接下來，你打算", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[2].totalDialogs = 2;
    }
    scenes[2].currentDialogIndex = 0;
    scenes[2].currentBackgroundIndex = 0;
    /*
    printf("獲得道具:受害者資料(圖)\n");
    printf("    小美，女性，30歲，小學老師，長捲髮\n");
    printf("    小麗，女性，18歲，學生，長捲髮\n");
    printf("    小漂，女性，32歲，家庭主婦，長捲髮\n");
    printf("    小亮，女性，23歲，服務員，長捲髮\n");
    
    printf("警長好感2\n");
    likes.Police += 2;

    if(Becca == 1)
        {
            strncpy(character2, "picture/Becca.jpg", 1000);
            strncpy(caption,  "你發現受害者都是長捲髮，不曉得是不是巧合，此刻你的腦海莫名出現 Becca的臉。\n這時有一個警員火急火燎的衝進辦公室，大喊連環殺人魔又下手了，而受害 者的照片赫然是Becca!\n你決定:", 1000);

            Option options[] =
            {
                {"(1)馬上告訴警長自己的猜:連續殺人犯很有可能是對長捲髮有執念，且昨天載Becca回去的男朋友很可疑!", NULL, {WINDOW_WIDTH - 300, 100, 200, 50}, 17},
                {"(2)趕快離開並偷偷去找東哥請求他的幫助", NULL, {WINDOW_WIDTH - 300, 200, 200, 50}, 18}
            };
            int totalOptions = sizeof(options) / sizeof(options[0]);

            scenes[0].dialogs[2].options = options;
            scenes[0].dialogs[2].totalOptions = totalOptions;

            choice = option.nextSceneIndex;
            character2 = NULL;
            story(choice,scenes);
            return;
        }
    */

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

    /*
        printf("流浪漢好感 3\n");
        likes.Street += 3;
    */

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"流浪漢感激地看了你一眼，開始狼吞虎嚥，這時你才發現它少了一條右臂 東哥好感 2\n你決定... <likes Street 2>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
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
    /* 如果有 becca
        printf("東哥好感2\n");
        printf("Becca好感1\n");
        likes.Don += 2;
        likes.Becca += 1;
        printf("離開之前，東哥偷偷遞給你一張紙條，上面寫著:有事第一個找我。\n");
        printf("獲得道具:東哥的紙條\n");
        backpack.donpaper +=1;
    */
    /* 如果沒有 becca
        printf("東哥好感2\n");
        likes.Don += 2;
    */
    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他說他叫東哥，原本在鎮上開一間水果店，經歷了一些意外從此消沉。你聽到後沒說什麼，給了他一些零錢後讓他有事可以找你便離開了。\n東哥好感 2 <likes Street 2>\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
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
        scenes[story].dialogs[0] = (Dialog){"警長表示兇手將受害者的頭皮割開，為了不對她們造成陰影才不讓他們看。你表示理解，同時也對殺人犯的手段表示不寒而慄。\n線索就停在了這裡，你發現推理真的沒有想像簡單。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[2].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

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
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他看了你一眼，輕描淡寫的帶過了。\n 東哥好感 -1 <likes Street -1>\n很快地結束飯局，你回到租屋處整理資料。發現甚麼都整理不出來", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38;

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
        scenes[story].dialogs[0] = (Dialog){"你發現什麼都整理不出來，推理好難。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

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
    /*
        printf("流浪漢好感2\n");
        likes.Street += 2;
    */

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"他開始狼吞虎嚥，這時你才發現它少了一條右臂，你決定..\n東哥好感 2<likes Street 2>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 10;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)裝沒看到，跟他閒話家常",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},35};
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

    /* 如果有 becca
        printf("Becca好感1\n");
        likes.Becca += 1;
    */

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/東哥.png", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"對世界漠視的的態度使你無法獲取線索", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

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

    // scene[12]
    story = 12;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/殯儀館.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/法醫.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"法醫表示兇手將死者的頭皮割開，手段非常殘忍，而且兇手將整片頭皮連同頭髮一起帶走，每位受害者都是如此，這令他感到十分不解。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 11;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)回警局向警長報告這個發現",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},13};
        scenes[story].options[1] = (Option){"(2)到大街上找別的線索",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},14};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[13]
    story = 13;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"警長若有所思地看了你一眼，然後告訴你這個案子有其他專人處理，不方便\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[14]
    story = 14;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/熱鬧大街.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/法醫.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"發現路上有很多學生，看一下時間才發現剛好放學了，你覺得進一所附近的高中看看。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 14;

    scenes[story].options = (Option*)calloc(3 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)去生態池看看",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},16};
        scenes[story].options[1] = (Option){"(2)去教學區看",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},21};
        scenes[story].options[2] = (Option){"(3)去體育館看",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},26};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 3;

    // scene[15]
    // 為 backgrounds 動態分配記憶體
    scenes[15].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[15].backgrounds != NULL) {
        scenes[15].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[15].totalBackgrounds = 1;
    }

    /*
        printf("警長給了你沒有照片的個人資料。\n");
        printf("    小美，女性，30歲，小學老師，長捲髮\n");
        printf("    小麗，女性，18歲，學生，長捲髮\n");
        printf("    小漂，女性，32歲，家庭主婦，長捲髮\n");
        printf("    小亮，女性，23歲，服務員，長捲髮\n");
        backpack.nopicture += 1;
        printf("警長好感1");
        likes.Police += 1;
    }
    */

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
        scenes[15].dialogs[0] = (Dialog){"獲得道具 : 沒有圖的受害者名單 \n你沒有從中得到任何發現......\n接下來你決定...<backpack nopicture 1>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[15].characters, 1, NULL, 0, NULL};
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

    // printf("15 ok\n");

    // scene[16]
    story = 16;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/生態池.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].backgrounds[1] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 2;
    }
    /*
        printf("becca好感3\n");
        likes.girl += 3;
    */

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].characters[1] = (Asset){"source/character/東哥.png", NULL, {WINDOW_WIDTH-250, 200, 250, 400}};
        scenes[story].totalCharacters = 2;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你來到生態池，剛好看到一個女孩失足掉進水裡，沒有過多猶豫，你直接上前救援。\n Becca 好感 3 <likes Becca 3>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].dialogs[1] = (Dialog){"女孩叫做Becca，你們去了一間麵店，發現旁邊有一個流浪漢蹲坐在路邊，看著前面雙眼無神。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 1, scenes[story].characters, 2, NULL, 0, NULL};
        scenes[story].totalDialogs = 2;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 40;
    // Becca = 1;

    scenes[story].options = (Option*)calloc(3 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)Hi ! 請問你可以幫我吃小菜嗎，不小心點太多了",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},41};
        scenes[story].options[1] = (Option){"(2)先生你很餓吧?我請你吃飯吧!",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},9};
        scenes[story].options[2] = (Option){"(3)不理他",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 70, 400, 50},35};
    }
    scenes[story].dialogs[1].options = scenes[story].options;
    scenes[story].dialogs[1].totalOptions = 3;

    // scene[17]
    story = 17;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }
    

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"警長看了你一眼，告訴你接下來是他們的辦公時間，請你離開。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38;
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[18]
    story = 18;
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
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"東哥沒多想直接告訴你，其實他的手是被他的好兄弟Kevin砍斷的，因為他撞見了Kevin的殺人現場，逃跑過程被砍斷了一隻手，這些年為了不要被認出，他一直裝做乞丐。然而昨天他發現Becca的男友竟就是Kevin!\n他說Kevin在母親過世後開始有瘋魔的狀態，只要看見長捲髮都會認為母親還在，因此開始不斷找長捲髮的女性下手。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 39; // end game // 贏了 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // printf("18 ok\n");

    // scene[19] 需要修改 
    story = 19;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"接下來你決定來到警局尋求當地警察的幫助。當地的警察聽到你要調查這起案件非常開心，因為這已經讓他們頭痛很久了，經過討論，他們決定讓警局裡德高望重的警長協助你調查。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 18;
    // Becca = 1;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)你好，我是一個新手記者，剛入職這個行業三個月，希望我們合作愉快。\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},2};
        scenes[story].options[1] = (Option){"(2)你好，我的姊姊是這起事件的受害者之一，所以請你一定要把所有資訊都告訴我，我定會盡所能協助破案。",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},15};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[20] 
    story = 20;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/晚上的公園.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你只聽到Becca故作冷靜地說:你帶我來和平公園做什麼.....電話馬上被掛斷，你頓感不妙，馬上驅車前往和平公園。\n來到公園，你一邊喊著Becca一邊往深處走，這時你踩到一個軟綿綿的東西，低頭一看竟然是倒在血泊中的Becca!\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[21] 
    story = 21;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/教學區.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }
    /*
        printf("獲得道具:Becca的藍芽耳機");
        backpack.airpods += 1;
    */
    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"來到教學區，你發現因為現在下課了，附近空無一人，但地上有一副遺落的藍芽耳機，打開後發現他的名稱是Becca\n 獲得道具 : Becca 的藍芽耳機 <backpack airpod 1>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].dialogs[1] = (Dialog){"撿起來後你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 2;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 18;
    /*
        printf("獲得道具:Becca的藍芽耳機");
        backpack.airpods += 1;
    */

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)發上學校尋物平台\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},22};
        scenes[story].options[1] = (Option){"(2)先收著",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},25};
    }
    scenes[story].dialogs[1].options = scenes[story].options;
    scenes[story].dialogs[1].totalOptions = 2;

    // scene[22] 
    story = 22;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/教學區.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }
    // printf("22 ok\n");

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"果然有人來聯絡你，一個叫Becca的女生跟你約在校門口見面。\n Becca 好感 2 <likes Becca 2>", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 18;
    /*
        printf("Becca好感2");
        likes.Becca += 2;
    */

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)最近關於連續殺人嫌犯，你有聽到什麼消息嗎?\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},23};
        scenes[story].options[1] = (Option){"(2)你的名字很好聽",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},24};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[23] 
    story = 23;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/教學區.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"Becca表示有聽說但沒頭緒。為了感謝你他決定請你吃飯。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 9;
    /*
        printf("Becca好感2");
        likes.Becca += 2;
    */

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)最近關於連續殺人嫌犯，你有聽到什麼消息嗎?\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},23};
        scenes[story].options[1] = (Option){"(2)你的名字很好聽",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},24};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[24] 
    story = 24;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/pure_black.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"這不是把妹時間，請公事公辦。\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 
    /*
        printf("Becca好感2");
        likes.Becca += 2;
    */
    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[25] 
    story = 25;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"肚子餓了，你決定去吃飯。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 24;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[25] 
    story = 25;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"肚子餓了，你決定去吃飯。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 3;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[26] 
    story = 26;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"現在正好是女籃的比賽時間，其中一個短髮女孩引起了你的注意，他自信張揚，籃球在他手中劃過完美的三分弧線。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 23;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)上前搭話\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},27};
        scenes[story].options[1] = (Option){"(2)在旁邊看",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},32};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // printf("26 ok\n");

    // scene[27] 
    story = 27;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"現在正好是女籃的比賽時間，其中一個短髮女孩引起了你的注意，他自信張揚，籃球在他手中劃過完美的三分弧線。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 23;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)嗨!可以跟你切磋一下嗎?\n",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},28};
        scenes[story].options[1] = (Option){"(2)你猜我什麼星座?是為你量身訂做 ( 比心",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},31};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[28] 
    story = 28;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"在打球過程中你知道她叫Jenny，原本留著一頭長髮，後來因為投入熱愛的籃球而剪去，前男友卻為此跟她分手。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 23;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)太誇張了吧!他到底是喜歡你還是喜歡你的頭髮??",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},29};
        scenes[story].options[1] = (Option){"(2)其實我覺得他這樣也有道理",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},30};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[29] 
    story = 29;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].characters[1] = (Asset){"source/character/Becca.jpg", NULL, {WINDOW_WIDTH-250, 200, 250, 400}};
        scenes[story].totalCharacters = 2;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"Jenny神秘兮兮地湊上來，說前男友還真的可能是喜歡他的頭髮，因為在他們分手後他又很快地交了新女友，正是他的閨密Becca，而Becca也是留一頭長捲髮。", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 3;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[30] 
    story = 30;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"Jenny大傻眼，直接帶著她的球離開了\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

    scenes[story].options = NULL;
    scenes[story].dialogs[1].totalOptions = 0;

    // scene[31] 
    story = 31;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/體育場.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"她嫌惡的看了你一眼，直接轉頭離開\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game

    scenes[story].options = NULL;
    scenes[story].dialogs[1].totalOptions = 0;

    // scene[32] 
    story = 32;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/pure_black.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Jenny.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"對世界漠視的態度使你無法找到更多有用的線索\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 

    scenes[story].options = NULL;
    scenes[story].dialogs[1].totalOptions = 0;

    // scene[33]
    story = 33;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(2 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].characters[1] = (Asset){"source/character/警長.jpg", NULL, {WINDOW_WIDTH - 250, 200, 250, 400}};
        scenes[story].totalCharacters = 2;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"警長給了你四名受害者的個人資料。\n獲得道具 : 受害者資料 <backpack picture 1>\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, &(scenes[story].characters[1]), 1, NULL, 0, NULL};
        scenes[story].dialogs[0] = (Dialog){"你發現受害者都是長捲髮，不曉得是不是巧合，此刻你的腦海莫名出現 Becca的臉。\n這時有一個警員火急火燎的衝進辦公室，大喊連環殺人魔又下手了，而受害 者的照片赫然是Becca!\n你決定:", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 33;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)太誇馬上告訴警長自己的猜想:連續殺人犯很有可能是對長捲髮有執念，且昨天載Becca回去的男朋友很可疑!",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},17};
        scenes[story].options[1] = (Option){"(2)趕快離開並偷偷去找東哥請求他的幫助",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},18};
    }
    scenes[story].dialogs[1].options = scenes[story].options;
    scenes[story].dialogs[1].totalOptions = 2;

    // scene[34]
    story = 34;
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
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"東哥表示Becca只是一個沒見過的女孩子，他幫不上什麼忙。\n看來你沒有當偵探的潛力......\nGAME OVER...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 38; // end game 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[35]
    story = 35;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/麵攤.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {WINDOW_WIDTH - 250, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"很快地結束飯局，Becca的男友來接他離開，秉持著交朋友的態度，你們互打電話加了號碼。\n回租屋處你累的直接睡了，到了半夜，迷糊中被電話吵醒，看見來電顯示竟然是Becca，你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 33;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)超想睡，直接把電話掛了",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},42};
        scenes[story].options[1] = (Option){"(2)接起來",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},20};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[36]
    story = 36;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/警長.jpg", NULL, {WINDOW_WIDTH - 250, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc(2 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你沒有從中得到任何發現......\n接下來你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 33;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)肚子餓了先去吃飯",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},3};
        scenes[story].options[1] = (Option){"(2)去受害者的家中拜訪",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},11};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scene[37] 
    story = 37;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/死亡.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你還來不及大喊，馬上感覺到脖子上有冰涼的觸感，低頭一看竟是一把刀，耳邊傳來Becca男友的聲音:被你發現了.....\n隨後你的脖子被一抹，Becca男友隨後自殺，你的任務就此失敗。\nGAME OVER...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 0; // end game 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    
    // scene[38] 
    story = 38;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/重刷.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"看來你沒有當偵探的潛力......\nGAME OVER...\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 0; // end game 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[39] 
    story = 39;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/勝利.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
        scenes[story].totalBackgrounds = 1;
    }

    // 為 characters 動態分配記憶體
    scenes[story].characters = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].characters != NULL) {
        scenes[story].characters[0] = (Asset){"source/character/Becca.jpg", NULL, {0, 200, 250, 400}};
        scenes[story].totalCharacters = 1;
    }

    // 為 dialogs 動態分配記憶體
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"你很疑惑為什麼東哥不向上舉報，東哥只說了句:Kevin的父親就市警局最德高望重的警長，他會替他掩蓋好一切罪刑並保護他。\n你在此終於得知了真相，但要怎麼對抗警局的勢力又是另外一個故事了。\nYOU WIN???", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 0; // end game 
    // Becca = 1;

    scenes[story].options = NULL;
    scenes[story].totalOptions = 0;

    // scene[40] 
    story = 40;
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
    scenes[story].dialogs = (Dialog*)calloc( 1 , sizeof(Dialog));
    if (scenes[story].dialogs != NULL) {
        scenes[story].dialogs[0] = (Dialog){"流浪漢感激地看了你一眼，開始狼吞虎嚥，這時你才發現它少了一條右臂，你決定...", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 0; // end game 
    // Becca = 1;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)裝沒看到，跟他閒話家常",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},41};
        scenes[story].options[1] = (Option){"(2)問他的右臂發生了什麼事",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},7};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scenes[41]
    story = 41;
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
        scenes[story].dialogs[0] = (Dialog){"他說他叫東哥，原本在鎮上開一間水果店，經歷了一些意外從此消沉。你聽到後沒說什麼，給了他一些零錢後讓他有事可以找你便離開了。\n東哥好感 2 <likes Street 2>\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].dialogs[1] = (Dialog){"回租屋處你累的直接睡了，到了半夜，迷糊中被電話吵醒，看見來電顯示竟然是Becca，你決定\n", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, NULL, 0, NULL, 0, NULL};
        scenes[story].totalDialogs = 2;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 8;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)超想睡，直接把電話掛了",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 170, 400, 50},42};
        scenes[story].options[1] = (Option){"(2)接起來",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},20};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;

    // scenes[42]
    story = 42;
    // 為 backgrounds 動態分配記憶體
    scenes[story].backgrounds = (Asset*)calloc(1 , sizeof(Asset));
    if (scenes[story].backgrounds != NULL) {
        scenes[story].backgrounds[0] = (Asset){"source/image/警局.jpg", NULL, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}};
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
        scenes[story].dialogs[0] = (Dialog){"接下來你決定來到警局尋求當地警察的幫助。\n這時有一個警員火急火燎的衝進辦公室，大喊連環殺人魔又下手了，而受害者的照片赫然是Becca!", NULL, {0, WINDOW_HEIGHT - DIALOG_HEIGHT, WINDOW_WIDTH, DIALOG_HEIGHT}, 0, scenes[story].characters, 1, NULL, 0, NULL};
        scenes[story].totalDialogs = 1;
    }
    scenes[story].currentDialogIndex = 0;
    scenes[story].currentBackgroundIndex = 0;
    scenes[story].nextSceneIndex = 8;

    scenes[story].options = (Option*)calloc(2 , sizeof(Option));
    if(scenes[story].options != NULL){
        scenes[story].options[0] = (Option){"(1)馬上告訴警長自己的猜:連續殺人犯很有可能是對長捲髮有執念，且昨天載Becca回去的男朋友很可疑!",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 200, 400, 50},17};
        scenes[story].options[1] = (Option){"(2)趕快離開並偷偷去找東哥請求他的幫助",NULL,{WINDOW_WIDTH-400, WINDOW_HEIGHT - DIALOG_HEIGHT - 120, 400, 50},18};
    }
    scenes[story].dialogs[0].options = scenes[story].options;
    scenes[story].dialogs[0].totalOptions = 2;
}

/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int register_user(int userID){
    if(userID > max_id){
        printf("User id %d is greater than max_id (%d)\n",userID,max_id);
        return FALSE;
    }
    if(HashTable_Lookup(userID) != NULL){
        printf("User with UID %d already exists\n",userID);
        return FALSE;
    }
    if(total_users > max_users){
        printf("Max user limit reached\n");
        return FALSE;
    }
    int index = hash(userID);
    user_ptr new_user = CreateUser(userID);
    if(user_hashtable_p[index] == NULL){
        user_hashtable_p[index] = new_user;
    }else{
        new_user->next = user_hashtable_p[index];
        user_hashtable_p[index] = new_user;
    }
    total_users++;
    //RegUserPr(userID);
    return TRUE;
}

/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int unregister_user(int userID){
    int index = hash(userID);
    user_ptr curr = user_hashtable_p[index];
    user_ptr prev = curr;
    /*an einai o prwtos sthn lista, proxwra ton head, diegrapse to istoriko tou xristi, kane ton histoyr pointer null kai free ton "user"*/
    if(curr!=NULL && curr->userID==userID){
        user_hashtable_p[index] = (user_hashtable_p[index])->next;
        DeleteTree(curr->history);
        curr->history = NULL;
        free(curr);
        printf("User %d Deleted\n",userID);
        return TRUE;
    }
    /*kane traverse to chain tou sugkekrimenou index mexri na vreis ton user kai krata pliroforia gia ton prev*/
    while(curr!=NULL && curr->userID!=userID){
        prev = curr;
        curr = curr->next;
    }
    /*an eftase sto telos ths listas kai den ton petuxe kapou, tote den uparxei o xrhsths se auto to chain, ara kai katholou*/
    if(curr == NULL){
        printf("User %d not found\n", userID);
        return FALSE;
        /*alliws an einai kapoios endiamesos komvos proxwra ton prev kai kane thn diagrafh*/
    }else{
        prev->next = curr->next;
    }
    DeleteTree(curr->history);
    curr->history = NULL;
    free(curr);
    printf("User %d Deleted\n", userID);
    return TRUE;
}


/*Leaf oriented binary search trees*/
/*Diagrafh aristerou paidiou: ftanw sto paidi, krataw voithitiko pointer ston patera tou, kanw free to paidi, vazw ton patera tou patera tou na deixnei sto deksi paidi kai kanw free ton patera*/
/*Diagrafh deksiou paidiou: ftanw sto paidi, krataw voithitiko pointer ston patera tou, kanw free to paidi, vazw ton patera tou patera na deixnei sto aristero paidi kai kanw free kai ton patera*/

/**
* @brief Add new movie to new release binary tree.
* Create a node movie and insert it in 'new release' binary tree.
*
* @param movieID The new movie identifier
* @param category The category of the movie
* @param year The year movie released
*
* @return 1 on success
*         0 on failure
*/

int add_new_movie(int movieID, Category category, int year){
    Newmovie_ptr newnode;
    Newmovie_ptr temp = NULL,Prev = NULL;
    newnode = CreateNewMovie(movieID,category,year);
    /*an to dentro newn kukloforiwn einai adeio, apla vale to prwto stoixeio ws riza*/
    if(IsEmptyTree()){
        new_releases = newnode;
    }else{
        /*temp pointer deixnei sthn arxh tou dentrou newn kukloforiwn*/
        temp = new_releases;
        while(temp != NULL){
            /*ean kathws kanei traverse to dentro vrei ton idio komvo, epistrefei FALSE*/
            if(temp->movieID == movieID){
                printf("Movie with ID %d already exists\n",movieID);
                free(newnode);
                return FALSE;
                /*alliws krata "parent" pointer gia na ginei swsta h eisagwgh*/
            }else{
                Prev = temp;
                if(movieID < temp->movieID){
                    temp = temp->lc;
                }else{
                    temp = temp->rc;
                }
            }
        }
        /*den mporei giati to kanw handle apo panw alla kalou kakou...*/
        if(Prev!=NULL){
            if(Prev->movieID > movieID){
                Prev->lc = newnode;
            }else{
                Prev->rc = newnode;
            }
        }
    }
    printf("\nNew releases Tree:\n\tNew_releases>: ");
    Information(new_releases);
    fflush(stdout);
    return TRUE;
}

/**
* @brief Distribute the movies from new release binary tree to the array of categories.
*
* @return 0 on success
*         1 on failure
*/

int distribute_movies(void){
    Newmovie_ptr cnm = NULL;
    MovieCategory_ptr category_root = NULL;
    int i;
    int n;
    int index;
    /*cnm = current new movie...*/
    while((cnm = RemoveNewMovie(&new_releases))){
        /*ousiastika ta vazei "opws nanai" sto dentro ths kathe kathgorias, "ksekolontas"
         *kathe fora enan komvo apo to dentro twn new kukloforiwn kai kanontas ton free
         *meta apo peras ths diadikasias*/
        Movie_ptr newmovie = CreateMovie(cnm->movieID,cnm->year);
        category_root = categoryArray[cnm->category];
        InsertMovieCT(category_root,newmovie);
        free(cnm);
    }
    /*twra, gia kathe kathgoria, ftiakse enan pinaka me pointers pou o kathe
     *enas deixnei se mia tainia tou ekastote pinaka kathgoriwn
     *kai perna ta apo thn teleutaia sunarthsh wste to teliko
     *dentro na exei upsos O(logn)*/
    for(i=0;i<6;i++){
        n = totalnodes(categoryArray[i],categoryArray[i]->movie);
        Movie_ptr arr[n];
        for(int j=0;j<n;j++)
            arr[j] = NULL;
        index = 0;
        CreateTable(categoryArray[i],categoryArray[i]->movie,&index,arr);
        categoryArray[i]->movie = OlognDistributedTree(categoryArray[i],arr,0,n-1);
    }
    PrintMoviesD();
    printf("\nDONE\n");
    return TRUE;
}

/**
* @brief User rates the movie with identification movieID with score
*
* @param userID The identifier of the user
* @param category The Category of the movie
* @param movieID The identifier of the movie
* @param score The score that user rates the movie with id movieID
*
* @return 1 on success
*         0 on failure
*/

int watch_movie(int userID,Category category, int movieID, int score){
    Movie_ptr found_movie = NULL;
    int flag = FALSE;
    int index;
    UserMovie_ptr curr = NULL;
    user_ptr User = NULL;
    UserMovie_ptr tmp = NULL;
    UserMovie_ptr newusermovie = NULL;
    if(!search_movie(movieID,category)){
        return FALSE;
    }
    found_movie = categoryArray[category]->movie;
    while(found_movie->movieID!=-1){
        if(found_movie->movieID==movieID){
            found_movie->sumScore+=score;
            found_movie->watchedCounter++;
            newusermovie = (UserMovie_ptr)malloc(sizeof(UserMovie));
            newusermovie->category = category;
            newusermovie->movieID = movieID;
            newusermovie->score = score;
            newusermovie->parent = newusermovie->lc = newusermovie->rc = NULL;
            /*insert sto leaf-oriented tou user*/
            index = hash(userID);
            User = FindUser(index,userID);
            if(User==NULL)
                break;
            /*an den uparxei oute riza kai einai adeio to watch history*/
            if(User->history==NULL){
                User->history = newusermovie;
            }else{
                /*exw pragmata mesa, osa kai na einai auta, prepei na kanw traverse
                 * to dentro kai na vrw to katallilo fulo gia to insertion*/
                curr = User->history;
                while(curr->lc != NULL && curr->rc !=NULL){
                    if(newusermovie->movieID<curr->movieID)
                        curr = curr->lc;
                    else
                        curr = curr->rc;
                }
                if(newusermovie->movieID<curr->movieID)
                    InsertLeftLOBST(newusermovie,curr);
                else
                    InsertRightLOBST(newusermovie,curr);
            }
            /*Egine to insertion opote h print mou apeksw prepei na to kserei, exw 48 xiliades if edw mesa...*/
            flag = TRUE;
            break;
        }
        /*den thumamai kan poio dentro einai pio, to dentro pou psaxnw tis tainies,
         *kane traverse mexri na vreis thn swsth*/
        if(movieID<found_movie->movieID)
            found_movie = found_movie->lc;
        else
            found_movie = found_movie->rc;
    }
    if(flag){
        tmp = User->history;
        printf("History Tree of user %d:",User->userID);
        PrintLOBST(tmp,movieID);
        printf("\nDONE\n");
        fflush(stdout);
    }
    return TRUE;
}



/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

int filter_movies(int userID, int score){
    int numMovies = 0;
    int i;
    MovieCategory_ptr Cat = NULL;
    Movie_ptr curr_movie = NULL;
    Movie_ptr* Movie_array = NULL;
    /*psiloirrelevant oi plhrofories tou user, rwthsa na mou poun giati xreiazetai kai den ematha......*/
    int index;
    index = hash(userID);
    int arr_index = 0;
    user_ptr User = FindUser(index,userID);
    if(!User){
        printf("User %d does not exist, filter movies is canceled\n",userID);
        return FALSE;
    }
    /*prwth diaperash olwn twn kathgoriwn gia na vrw poses tainies exoun avg score megalutero apo to score tou orismatos.*/
    for(i=0;i<6;i++){
        Cat = categoryArray[i];
        curr_movie = Cat->movie;
        ScoreCounterTRVRS(&numMovies,Cat,&curr_movie,score);
    }
    Movie_array = (Movie_ptr*)malloc(numMovies*sizeof(Movie_ptr));
    if(!Movie_array){
        printf("Memory allocation error");
        exit(-1);
    }
    for(i=0;i<6;i++){
        Cat = categoryArray[i];
        curr_movie = Cat->movie;
        InitMovieArray(&arr_index,Cat,&curr_movie,score,Movie_array);
    }
    printf("F %d %d\n", userID,score);
    if(numMovies==0){
        printf("\n\tNo movies had an average score above %d\n",score);
    }else{
        HeapSort(Movie_array,numMovies);
        for(i=0;i<numMovies;i++){
            printf("\tmid=%d, Average score=%.2f\n",Movie_array[i]->movieID, GetAVGScore(Movie_array[i]));
            fflush(stdout);
        }
    }
    free(Movie_array);
    printf("DONE\n");
    return TRUE;
}


/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

int user_stats(int userID){
    user_ptr User = NULL;
    int index = hash(userID);
    int ScoreSum = 0;
    int counter = 0;
    UserMovie_ptr curr_movie = NULL;
    User = FindUser(index,userID);
    if(!User){
        printf("User %d not found",userID);
        return FALSE;
    }
    /*go to the leftmost leaf*/
    if(User->history==NULL){
        printf("Empty tree");
        return FALSE;
    }
    curr_movie = User->history;
    while(curr_movie->lc!=NULL && curr_movie->rc!=NULL){
        curr_movie = curr_movie->lc;
    }
    while(curr_movie!=NULL){
        counter++;
        ScoreSum += curr_movie->score;
        curr_movie = FindNextLeaf(curr_movie);
    }
    if(counter==0)
        printf("\t%d - 0\n",userID);
    else{
        printf("\t%d %.2f\n",userID,(float)ScoreSum/(float)counter);
    }
    printf("DONE\n");
    return TRUE;
}

UserMovie_ptr FindNextLeaf(UserMovie_ptr curr_node) {
    UserMovie_ptr next = NULL;

    /*Checkare a o current_node exei deksi paidi. An exei,
     *tote to epomeno fullo einai o aristeroteros komvos
     *tou deksiou upodentrou*/
    if (curr_node->rc != NULL) {
        next = curr_node->rc;
        /*navigate mexri ton aristerotero komvo tou deksiou upodentrou*/
        while (next->lc != NULL) {
            next = next->lc;
        }
    } else {
        /*Alliws, an den uparxei deksi paidi, phgaine pros ta panw(parent)*/
        next = curr_node->parent;
        /*Sunexise na phgaineis pros ta panw mexri na vreis
         *ton komvo apo ton opoio irthame apo aristera*/
        while (next != NULL && curr_node == next->rc) {
            curr_node = next;
            next = next->parent;
        }
    }

    /*Sigourepsou oti o next einai fullo. An oxi, epanalave thn diadikasia*/
    while (next != NULL && (next->lc != NULL || next->rc != NULL)) {
        /*An to next exei deksi paidi, kane to idio gia to deksi upodentro*/
        if (next->rc != NULL) {
            next = next->rc;
            while (next->lc != NULL) {
                next = next->lc;
            }
        } else {
            /*alliws phgaine pros ta panw kai epanalave*/
            curr_node = next;
            next = next->parent;
            while (next != NULL && curr_node == next->rc) {
                curr_node = next;
                next = next->parent;
            }
        }
    }

    return next;
}

user_ptr FindUser(int index,int uid){
    user_ptr tmp = user_hashtable_p[index];
    while(tmp!=NULL && tmp->userID!=uid){
        tmp = tmp->next;
    }
    return tmp;
}



/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

int search_movie(int movieID, Category category){
    Movie_ptr tmp = NULL;
    tmp = categoryArray[category]->movie;
    if(tmp==NULL)
        return FALSE;
    while(tmp->movieID!=-1){
        if(tmp->movieID==movieID){
            printf("\nI %d %s %d\nDONE\n",movieID, GetCategoryName(category),tmp->year);
            return TRUE;
        }
        if(movieID<tmp->movieID)
            tmp = tmp->lc;
        else
            tmp = tmp->rc;
    }
    return FALSE;
}

/**
* @brief Prints the movies in movies categories array.
* @return 1 on success
*         0 on failure
*/

int print_movies(void){
    Movie_ptr curr = NULL;
    printf("\nM\nMovie Category Array: \n");
    int i;
    for(i=0;i<6;i++) {
        curr = categoryArray[i]->movie;
        printf("\t%s:", GetCategoryName(i));

        if (curr == categoryArray[i]->sentinel) {
            printf("None\n");
            continue;
        }
        PrintD(curr, categoryArray[i]);
        printf("\n");
        fflush(stdout);
    }
    printf("\nDONE\n");
    return TRUE;
}

/**
* @brief Prints the users hashtable.
* @return 1 on success
*         0 on failure
*/

int print_users(void){
    int i;
    user_ptr curr_user = NULL;
    UserMovie_ptr movie = NULL;
    for(i=0;i<hashtable_size;i++){
        if(user_hashtable_p[i]==NULL)
            continue;
        printf("\nChain %d of Users: ",i);
        curr_user = user_hashtable_p[i];
        do{
            printf("\n\tUser %d\n",curr_user->userID);
            if(curr_user->history!=NULL){
                printf("\tHistory Tree:");
                movie = curr_user->history;
                PrintUsersHistory(movie);
            }else{
                printf("\tEmpty History Tree\n");
            }
            curr_user = curr_user->next;
        }while(curr_user!=NULL);
    }
    printf("\nDONE\n");
    return TRUE;
}



void initialize_hash_table(){
    hashtable_size = BinaryPrimeSearch();
    user_hashtable_p = (user_ptr *)malloc(hashtable_size*sizeof(user_ptr));
    for(int i = 0;i<hashtable_size;i++){
        user_hashtable_p[i] = NULL;
    }
    printf("Hashtable size: %d\n",hashtable_size);
}

int hash(int uid){
    if(a_gl==0 && b_gl==-1){
        while(a_gl==0)
            a_gl=rand()%p;
        b_gl=rand()%p;
    }
    return ((a_gl*uid+b_gl)%p)%hashtable_size;
}

/*Ousiastika kanei traverse ta chains tou kathe keliou tou hashtable. To */
user_ptr HashTable_Lookup(int uid){
    int index = hash(uid);
    user_ptr tmp = user_hashtable_p[index];
    while(tmp!=NULL){
        if(tmp->userID == uid){
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void RegUserPr(int uid){
    int index = hash(uid);
    printf("\tChain %d of Users: \n",index);
    user_ptr tmp = user_hashtable_p[index];
    while(tmp!=NULL){
        printf("\t\t%d\n",tmp->userID);
        tmp = tmp->next;
    }
    printf("\tDONE\n");
}

void free_hash_table(){
    user_ptr temp = NULL;
    for(int i=0;i<hashtable_size;i++){
        user_ptr current = user_hashtable_p[i];
        while(current != NULL){
            temp = current;
            current = current->next;
            DeleteTree(temp->history);
            free(temp);
            temp = NULL;
        }
    }
    free(user_hashtable_p);
    user_hashtable_p = NULL;
}

void NewReleasesInit(){
    new_releases = NULL;
}

int IsEmptyTree(){
    return new_releases == NULL;
}

void Information(Newmovie_ptr root){
    if(root==NULL)
        return;
    Information(root->lc);
    printf("%d, ",root->movieID);
    Information(root->rc);
}

Newmovie_ptr CreateNewMovie(int movieID, int category, int year){
    Newmovie_ptr newnode = NULL;
    newnode = (Newmovie_ptr)malloc(sizeof(Newmovie));
    if(newnode==NULL){
        printf("Malloc failed\n");
        return NULL;
    }
    newnode->movieID = movieID;
    newnode->year = year;
    newnode->category = category;
    newnode->lc = NULL;
    newnode->rc = NULL;
    newnode->sumScore = 0;
    newnode->watchedCounter = 0;
    return newnode;
}

user_ptr CreateUser(int userID){
    user_ptr new_user = (user_ptr)malloc(sizeof(User));
    if(new_user==NULL){
        /*mono edw to exw valei*/
        fprintf(stderr,"Malloc failed\n");
        exit(-1);
    }
    new_user->userID = userID;
    new_user->history = NULL;
    new_user->next = NULL;
    return new_user;
}

Movie_ptr CreateMovie(int movieId, int year){
    Movie_ptr newmovie = (Movie_ptr)malloc(sizeof(Movie));
    if(!newmovie){
        printf("Malloc failed\n");
        /*random number btw*/
        exit(118);
    }
    newmovie->movieID = movieId;
    newmovie->year = year;
    newmovie->sumScore = 0;
    newmovie->watchedCounter = 0;
    newmovie->lc = SENT;
    newmovie->rc = SENT;
    return newmovie;
}

Newmovie_ptr RemoveNewMovie(Newmovie_ptr *root) {
    if (*root == NULL) {
        return NULL;
    }

    /* If the current node is a leaf node*/
    if ((*root)->lc == NULL && (*root)->rc == NULL) {
        Newmovie_ptr leafNode = *root;
        *root = NULL;
        return leafNode;
    }

    Newmovie_ptr removedNode = NULL;
    /* Recursively remove leaf nodes from left and right subtrees*/
    removedNode = RemoveNewMovie(&((*root)->lc));
    if (removedNode != NULL) {
        return removedNode;
    }
    removedNode = RemoveNewMovie(&((*root)->rc));
    return removedNode;
}


void InsertMovieCT(MovieCategory_ptr cat, Movie_ptr newmovie){
    /*The node that is about to be inserted has its lc and rc pointers pointing to the sentinel*/
    newmovie->lc = cat->sentinel;
    newmovie->rc = cat->sentinel;
    Movie_ptr tmp = NULL,parent = NULL;

    /*if the category's root pointer(movie) is empty, meaning it points to sentinel then...*/
    if(cat->movie == cat->sentinel){
        cat->movie = newmovie;/*newnode becomes root*/
        return;
    }
    /*traverse to dentro kai vres pou tha mpei to neo matzafliki*/
    tmp = cat->movie;
    while(tmp!=cat->sentinel){
        parent = tmp;
        if(newmovie->movieID < tmp->movieID)
            tmp = tmp->lc;
        else
            tmp = tmp->rc;
    }
    /*aristero h deksi paidi.*/
    if(newmovie->movieID<parent->movieID){
        parent->lc = newmovie;
    }else{
        parent->rc = newmovie;
    }
}

void PrintMoviesD(){
    Movie_ptr curr = NULL;
    printf("\nMovie Category Array: ");
    int i;
    for(i=0;i<6;i++){
        curr = categoryArray[i]->movie;
        printf("\n\t%s: ", GetCategoryName(i));
        if(curr == categoryArray[i]->sentinel){
            printf("None");
            continue;
        }
        PrintD(curr,categoryArray[i]);
        fflush(stdout);
    }
}

void PrintD(Movie_ptr root, MovieCategory_ptr cat){
    if(root==NULL){
        return;
    }
    PrintD(root->lc,cat);
    if(root->movieID!=-1)
        printf("%d, ",root->movieID);
    PrintD(root->rc,cat);
}

int totalnodes(MovieCategory_ptr cat, Movie_ptr root){
    if(root==cat->sentinel)
        return 0;
    return 1+ totalnodes(cat,root->lc)+ totalnodes(cat,root->rc);
}

void CreateTable(MovieCategory_ptr cat,Movie_ptr root, int *index, Movie_ptr arr[]){
    if(root==cat->sentinel || index==NULL){
        return;
    }
    CreateTable(cat,root->lc,index,arr);
    arr[*index] = root;
    (*index)++;
    CreateTable(cat,root->rc,index,arr);
}

Movie_ptr OlognDistributedTree(MovieCategory_ptr cat, Movie_ptr arr[], int start,int end){
    if (start > end) {
        return cat->sentinel;
    }
    int mid = start + (end - start) / 2;
    Movie_ptr node = arr[mid];

    node->lc = OlognDistributedTree(cat, arr, start, mid - 1);
    node->rc = OlognDistributedTree(cat, arr, mid + 1, end);
    return node;
}


char* GetCategoryName(Category movieCategory){
    switch(movieCategory){
        case HORROR:return "HORROR";
        case COMEDY:return "COMEDY";
        case DRAMA:return "DRAMA";
        case DOCUMENTARY:return "DOCUMENTARY";
        case ROMANCE:return "ROMANCE";
        case SCIFI:return "SCI-FI";
    }
}

Movie_ptr FindMovie(int mid, Category cat){
    Movie_ptr tmp = categoryArray[cat]->movie;
    while(tmp->movieID!=-1){
        if(tmp->movieID==mid)
            return tmp;
        if(mid<tmp->movieID)
            tmp = tmp->lc;
        else
            tmp = tmp->rc;
    }
    return NULL;
}

void PrintUsersHistory(UserMovie_ptr movie){
    if(movie==NULL)
        return;
    PrintUsersHistory(movie->lc);
    if(movie->lc==NULL && movie->rc == NULL){
        printf("\n\t\t%d, Score=%d ",movie->movieID,movie->score);
    }
    PrintUsersHistory(movie->rc);
}

void PrintLOBST(UserMovie_ptr tmp,int mid){
    if(tmp==NULL)
        return;
    PrintLOBST(tmp->lc,mid);
    if(tmp->lc==NULL && tmp->rc == NULL){
        printf("\n\t%d, Score=%d ",tmp->movieID,tmp->score);
        if(tmp->movieID==mid)
            printf("(Inserted)");
    }
    PrintLOBST(tmp->rc,mid);
}


/*Kai se authn kai sthn InsertRight, ftiaxnw enan kainourio komvo, ton new 2.
 *Se kathe periptwsh tsekarw o current pointer prokeitai na kanei eisagwgh sthn riza kai ama to kanei apla ginetai
 *updated o komvos parent tou current. Either way, arxikopoiw ton new2 komvo me ta stoixeia pou prepei,
 *dhladh idio movieID me ton current komvo kai -1 sta upoloipa stoixeia, ftiaxnw ton parent tou new2
 *na deixnei ston parent tou curr afou tha ton antikatasthsei ws to paidi tou eventually...
 *Metaferw ton curr "katw" se aristero h deksi paidi tou new2, kai o pleon eswterikos komvos einai to new2.
 *O curr eksakolouthei na einai fullo, efoson oi lc,rc pointers tou einai akoma NULL, kai telos sundew sto deksi
 *h aristero paidi(analoga thn sunarthsh) to newnode wste na epiteuxthei h eisagwgh*/
void InsertLeftLOBST(UserMovie_ptr newusermovie,UserMovie_ptr curr){
    UserMovie_ptr new2 = (UserMovie_ptr) malloc(sizeof(UserMovie));
    if(!new2){
        printf("Adunamia desmeushs mnhmhs");
        exit(-1);
    }
    new2->movieID = curr->movieID;
    new2->score = curr->score;
    new2->category = curr->category;
    new2->lc = NULL;
    new2->rc = NULL;
    curr->score = -1;
    curr->category = -1;
    /*An kanw eisagwgh sthn riza, den mpainei sthn if*/
    new2->parent = curr;
    curr->rc = new2;
    curr->lc = newusermovie;
    newusermovie->parent = curr;
}


void InsertRightLOBST(UserMovie_ptr newusermovie, UserMovie_ptr curr){
    UserMovie_ptr new2 = (UserMovie_ptr)malloc(sizeof(UserMovie));
    if(!new2){
        printf("Adunamia desmeushs mnhmhs");
        exit(-1);
    }
    new2->movieID = curr->movieID;
    new2->score = curr->score;
    new2->category = curr->category;
    new2->parent = curr;
    new2->lc = NULL;
    new2->rc = NULL;
    /*An kanw eisagwgh sthn riza, den mpainei sthn if*/
    new2->parent = curr;
    newusermovie->parent = curr;
    curr->movieID = newusermovie->movieID;
    curr->rc = newusermovie;
    curr->lc = new2;
    curr->score = -1;
    curr->category = -1;
}

void ScoreCounterTRVRS(int *numMovies, MovieCategory_ptr cat, Movie_ptr *curr_movie, int score){
    if(*curr_movie==cat->sentinel)
        return;
    float mscore = 0;
    ScoreCounterTRVRS(numMovies,cat,&((*curr_movie)->lc),score);
    mscore = GetAVGScore(*curr_movie);
    /**/
    if(mscore>=(float)score){
        (*numMovies)++;
    }
    ScoreCounterTRVRS(numMovies,cat,&((*curr_movie)->rc),score);
}

void InitMovieArray(int *index,MovieCategory_ptr cat,Movie_ptr *curr_movie,int score,Movie_ptr arr[]){
    if(*curr_movie==cat->sentinel)
        return;
    float mscore;
    InitMovieArray(index,cat,&((*curr_movie)->lc),score,arr);
    mscore = GetAVGScore(*curr_movie);
    /*auto kai to apo panw, to eixa mscore>(float)score, alla den kserw ti thete giati h ekfwnhsh htan polu kakh se auto to shmeio...
     *thewrhsa oti oses tainies exoun 0 score, tha prepei na emfanizontai kai mono an valw >= tha emfanistoun sthn periptwhsh tou F mid 0*/
    if(mscore>=(float)score){
        arr[*index] = *curr_movie;
        (*index)++;
    }
    InitMovieArray(index,cat,&((*curr_movie)->rc),score,arr);
}

void DeleteTree(UserMovie_ptr root){
    if(root==NULL)
        return;
    DeleteTree(root->lc);
    DeleteTree(root->rc);
    free(root);
}

void DeleteCategoryTree(Movie_ptr root){
    if(root->movieID==-1)
        return;
    DeleteCategoryTree(root->lc);
    DeleteCategoryTree(root->rc);
    free(root);
}

float GetAVGScore(Movie_ptr mv){
    if(mv->watchedCounter!=0)
        return(float)mv->sumScore/(float)mv->watchedCounter;
    else
        return 0;
}

void Heapify(Movie_ptr arr[], int size, int i){
    /*thewrw oti h riza(i) einai to megalutero stoixeio*/
    int max = i;
    int left = 2*i+1;
    int right = 2*i+2;
    /*Check if the left child exists by comparing its index with the size of the heap
     *and if the left child's average score is greater than the root's average score.
     *Ama ta average scores einai idia, elegxw ta movieID kai taksinomw vash autwn gia ta 2 auta stoixeia mono
     *then update the max to be the left child.*/
    if (left < size) {
        if (GetAVGScore(arr[left]) > GetAVGScore(arr[max]) ||
            (GetAVGScore(arr[left]) == GetAVGScore(arr[max]) && arr[left]->movieID > arr[max]->movieID)) {
            max = left;
        }
    }
    if (right < size) {
        if (GetAVGScore(arr[right]) > GetAVGScore(arr[max]) ||
            (GetAVGScore(arr[right]) == GetAVGScore(arr[max]) && arr[right]->movieID > arr[max]->movieID)) {
            max = right;
        }
    }
    if (max != i) {
        Swap(&arr[i], &arr[max]);
        Heapify(arr, size, max);
    }
}

void Swap(Movie_ptr *node1, Movie_ptr *node2){
    Movie_ptr tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
}
/*Ftiaxnei ena max heap pou to megalutero stoixeio einai sthn riza. Epeita antalazei thn riza tou heap me to teleutaio stoixeio tou pinaka.
 *To megethos ths swrou meiwnetai kata 1, agnooume dhladh to teleutaio kai megalutero stoixeio kai kaleitai h heapify. H diadikasia epanalamvanetai
 *me to epomeno megalutero stoixeio na topotheteitai sto telos tou ekastote mh taksinomimenou upo-pinaka kathe fora kai telika, auksousa diatakash.*/
void HeapSort(Movie_ptr arr[], int size) {
    /*Ftiaxnei to max heap*/
    for (int i = size / 2 - 1; i >= 0; i--)
        Heapify(arr, size, i);
    /*kane extract ena ena stoixeio apo thn swro*/
    for (int i = size - 1; i > 0; i--) {
        /*metakinise to current root sto telos*/
        Swap(&arr[0], &arr[i]);
        /*ksanakalese thn max heapify sthn meiwmenh swro*/
        Heapify(arr, i, 0);
    }
}

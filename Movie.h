/*****************************************************
 * @file   Movie.h                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Header File for Data Structures (CS240b)   *
 * Project: Winter 2023						         *
 *****************************************************/
#ifndef MOVIE_FILE
#define MOVIE_FILE
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>


/*You can use this enum or you can choose to represent each category as an Integer. Its the same*/
typedef enum {
    HORROR,
    SCIFI,
    DRAMA,
    ROMANCE,
    DOCUMENTARY,
    COMEDY
}Category;

/**
* Structure defining a node of a new_realeases binary tree (dendro newvn kykloforiwn)
*/
typedef struct new_movie{
    int category;				/* The category of the movie*/
    int movieID; 				/* The movie identifier*/
    int year;					/* The year movie released*/
    int watchedCounter;	 		/* How many users rate the movie*/
    int sumScore;				/* The sum of the ratings of the movie*/
    struct new_movie *lc;	/* Pointer to the node's left child*/
    struct new_movie *rc;	/* Pointer to the node's right child*/
}Newmovie;


/**
 *Structure defining a node of movie binary tree (dendro tainiwn kathgorias)
*/
typedef struct movie{
    int movieID; 				/* The movie identifier*/
    int year;					/* The year movie released*/
    int watchedCounter;	 		/* How many users rate the movie*/
    int sumScore;				/* The sum of the ratings of the movie*/
    struct movie *lc;	/* Pointer to the node's left child*/
    struct movie *rc;	/* Pointer to the node's right child*/
}Movie;

/**
* Structure defining movie_category
*/

typedef struct movie_category{
    Movie *movie;
    Movie *sentinel;
}MovieCategory;


/**
* Structure defining a node of user_movie for history doubly linked binary
* tree (dentro istorikou)
*/
typedef struct user_movie{
    int movieID; 					/* The movie identifier*/
    int category;					/* The category of the movie*/
    int score;						/* The score of the movie*/
    struct user_movie *parent;		/* Pointer to the node's parent*/
    struct user_movie *lc;	/* Pointer to the node's left child*/
    struct user_movie *rc;	/* Pointer to the node's right child*/
}UserMovie;

/**
* Structure defining a node of users' hashtable (pinakas katakermatismou
* xrhstwn)
*/
typedef struct user {
    int userID; 			/* The user's identifier*/
    UserMovie *history;	/* A doubly linked binary tree with the movies watched by the user*/
    struct user *next;		/* Pointer to the next node of the chain*/
}User;

typedef User *user_ptr;
typedef Newmovie* Newmovie_ptr;
typedef Movie* Movie_ptr;
typedef UserMovie* UserMovie_ptr;
typedef MovieCategory* MovieCategory_ptr;
typedef Movie* Movie_ptr;
extern MovieCategory *categoryArray[6];  /* The categories array (pinakas kathgoriwn)*/
extern Newmovie *new_releases;    		   /* New releases simply-linked binary tree*/
extern User **user_hashtable_p;		   /* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
extern int hashtable_size;				   /* The size of the users hashtable, parsed from the command line (>0)*/
extern int max_users;					   /* The maximum number of registrations (users)*/
extern int max_id;   			   /* The maximum account ID */
extern int primes_g[170];
extern int p;
extern int total_users;
extern int a_gl,b_gl;
extern Movie_ptr SENT;



/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int register_user(int userID);

/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int unregister_user(int userID);

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

int add_new_movie(int movieID, Category category, int year);

/**
* @brief Distribute the movies from new release binary tree to the array of categories.
*
* @return 1 on success
*         0 on failure
*/

int distribute_movies(void);

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

int watch_movie(int userID, Category category,int movieID, int score);

/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

int filter_movies(int userID, int score);

/**
 * @brief Find the median score that user rates movies.
 *
 * @param userID The identifier of the user
 *
 * @return 1 on success
 *         0 on failure
 */

int user_stats(int userID);

/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

int search_movie(int movieID, Category category);

/**
* @brief Prints the movies in movies categories array.
* @return 1 on success
*         0 on failure
*/

int print_movies(void);

/**
* @brief Prints the users hashtable.
* @return 1 on success
*         0 on failure
*/

int print_users(void);

void initialize_hash_table();
int hash(int uid);
user_ptr HashTable_Lookup(int uid);
void free_hash_table();
void RegUserPr(int uid);
int BinaryPrimeSearch();
void NewReleasesInit();
int IsEmptyTree();
void Information(Newmovie_ptr root);
Newmovie_ptr CreateNewMovie(int movieID, int category, int year);
user_ptr CreateUser(int userID);
Movie_ptr CreateMovie(int movieId, int year);
void InsertMovieCT(MovieCategory_ptr root, Movie_ptr newmovie);
Newmovie_ptr RemoveNewMovie(Newmovie_ptr *root);
void PrintMoviesD();
void PrintD(Movie_ptr root,MovieCategory_ptr cat);
int totalnodes(MovieCategory_ptr cat, Movie_ptr root);
void CreateTable(MovieCategory_ptr  cat, Movie_ptr node, int* index, Movie_ptr arr[]);
Movie_ptr OlognDistributedTree(MovieCategory_ptr cat,Movie_ptr arr[], int start,int end);
char* GetCategoryName(Category movieCategory);
void InsertLeftLOBST(UserMovie_ptr newusermovie,UserMovie_ptr curr);
void InsertRightLOBST(UserMovie_ptr newusermovie, UserMovie_ptr curr);
void PrintLOBST(UserMovie_ptr tmp,int movieID);
void ScoreCounterTRVRS(int *numMovies, MovieCategory_ptr cat, Movie_ptr *curr_movie, int score);
void InitMovieArray(int *index,MovieCategory_ptr cat,Movie_ptr *curr_movie,int score, Movie_ptr arr[]);
void HeapSort(Movie_ptr arr[],int size);
void Heapify(Movie_ptr arr[],int size, int i);
void Swap(Movie_ptr *node1, Movie_ptr *node2);
float GetAVGScore(Movie_ptr mv);
UserMovie_ptr FindNextLeaf(UserMovie_ptr curr_node);
user_ptr FindUser(int index,int uid);
void PrintUsersHistory(UserMovie_ptr movie);
void DeleteTree(UserMovie_ptr root);
void DeleteCategoryTree(Movie_ptr root);

#endif
# Streaming Service Phase-B
Advanced Streaming Service Simulation, Written in C programming Language, part of Data Structures course

## Build and Run
Run using Makefile: <br>
```bash
make
./cs240StreamingServicePhaseb <testfile>
```
*afterwards* <br>
```bash
make clean
```

## Project Description
This is Phase B of the streaming service simulation project, implementing advanced data structures and algorithms for efficient movie and user management. The system provides sophisticated operations for managing users, movies, and viewing statistics with optimized performance characteristics.

### Key Components:
- **Users**: Managed via hash table with chaining for collision resolution
- **Movies**: Organized by categories using balanced binary search trees with sentinel nodes
- **Watch History**: Stored as leaf-oriented binary search trees (LOBST) per user
- **New Releases**: Temporary binary search tree for batch processing
- **Categories**: 6 predefined categories (0→5): Horror, SCI-FI, Drama, Romance, Documentary, Comedy

### Advanced Features:
- Universal hashing for user ID distribution
- Balanced tree construction for O(log n) movie operations
- Heap sort for filtered movie results
- Efficient leaf-oriented binary search trees for user history
- Batch movie distribution with O(n) complexity guarantee

## Data Structures Overview

### Hash Table (Users)
- **Implementation**: Array of chains with universal hashing
- **Hash Function**: ((a * uid + b) % p) % hashtable_size
- **Collision Resolution**: Separate chaining
- **Size**: Dynamically determined prime number > max_users

### Binary Search Trees (Movies)
- **Type**: Balanced BST with sentinel nodes
- **Balancing**: Achieved through array-based reconstruction after distribution
- **Sentinel**: Special node with movieID = -1 for boundary marking
- **Height**: O(log n) guaranteed after distribution phase

### Leaf-Oriented Binary Search Trees (User History)
- **Structure**: Internal nodes store routing information, leaves store actual data
- **Insertion**: Creates new internal nodes as needed
- **Traversal**: In-order traversal of leaves only
- **Parent Pointers**: Maintained for efficient navigation

## Event Operations

### 1. R (Register User) → `R <uid>`
- **Purpose**: Adds user with ID uid to the hash table
- **Constraints**: 
  - uid must be ≤ max_id
  - uid must be unique
  - Total users must be ≤ max_users
- **Complexity**: O(1) average case
- **Output**: Chain information for the user's hash bucket

### 2. U (Unregister User) → `U <uid>`
- **Purpose**: Removes user and cleans up all associated data
- **Operations**:
  - Removes user from hash table chain
  - Deletes entire watch history tree
  - Frees all allocated memory
- **Complexity**: O(h) where h is height of user's history tree
- **Output**: Confirmation of deletion

### 3. A (Add New Movie) → `A <mid> <category> <year>`
- **Purpose**: Adds movie to the new releases binary tree
- **Constraints**: 
  - movieID must be unique in new releases
  - Category must be 0-5
- **Structure**: Maintains BST property by movieID
- **Complexity**: O(log n) where n is number of new releases
- **Output**: Current state of new releases tree (in-order traversal)

### 4. D (Distribute Movies) → `D`
- **Purpose**: Moves all movies from new releases to category trees
- **Algorithm**:
  1. Extract all movies from new releases tree (leaf removal)
  2. Insert into appropriate category trees
  3. Rebuild category trees as balanced BSTs using array-based method
- **Complexity**: O(n) where n is total number of movies being distributed
- **Output**: Final state of all category trees

### 5. W (Watch Movie) → `W <uid> <category> <mid> <score>`
- **Purpose**: Records user's movie viewing and rating
- **Operations**:
  1. Verify movie exists in specified category
  2. Update movie's sum score and watch counter
  3. Insert viewing record into user's LOBST history
- **LOBST Insertion**: Creates internal nodes as needed for leaf placement
- **Complexity**: O(log m + log h) where m is movies in category, h is user's history size
- **Output**: User's updated history tree with new entry marked

### 6. F (Filter Movies) → `F <uid> <score>`
- **Purpose**: Finds all movies with average rating ≥ score across all categories
- **Algorithm**:
  1. Traverse all category trees to count qualifying movies
  2. Build array of movie pointers
  3. Sort using heap sort by average score (descending), then by movieID (ascending)
- **Complexity**: O(n log n) where n is total number of movies
- **Output**: Sorted list of movies with their average scores

### 7. Q (User Statistics) → `Q <uid>`
- **Purpose**: Calculates user's average movie rating
- **Algorithm**:
  1. Navigate to leftmost leaf in user's LOBST
  2. Traverse all leaves using FindNextLeaf() function
  3. Accumulate scores and count ratings
- **Complexity**: O(k) where k is number of movies rated by user
- **Output**: User's average rating with 2 decimal precision

### 8. I (Search Movie) → `I <mid> <category>`
- **Purpose**: Searches for specific movie in a category
- **Algorithm**: Standard BST search in specified category tree
- **Complexity**: O(log n) where n is movies in category
- **Output**: Movie details (ID, category name, year) if found

### 9. M (Print Movies) → `M`
- **Purpose**: Displays all movies organized by category
- **Output Format**: 
  - Category name followed by movie IDs in ascending order
  - "None" for empty categories
- **Traversal**: In-order traversal of each category tree
- **Complexity**: O(n) where n is total number of movies

### 10. P (Print Users) → `P`
- **Purpose**: Displays complete hash table with user information
- **Output Format**:
  - Chain number and all users in that chain
  - For each user: ID and complete watch history
  - History shown as leaf traversal with scores
- **Complexity**: O(u + h) where u is users, h is total history entries

## Test File Format
```
<event_code> <arg1> <arg2> <arg3> <arg4>
```

### Examples:
```
0 10          # Set max_users to 10
1 999         # Set max_id to 999, initialize structures
R 691         # Register user 691
P             # Print all users
A 1202 2 2003 # Add movie 1202, category Drama, year 2003
M             # Print all movies
I 1202 2      # Search for movie 1202 in Drama category
W 691 2 1202 8 # User 691 watches movie 1202, rates it 8
F 460 8       # Filter movies with average score >= 8
Q 691         # Get statistics for user 691
```

## Memory Management
- **Dynamic Allocation**: All structures use malloc/free
- **Cleanup**: Proper deallocation in destroy_structures()
- **Tree Deletion**: Recursive post-order traversal for tree cleanup
- **Hash Table**: Chain traversal for user cleanup

## Performance Characteristics
| Operation | Average Case | Worst Case |
|-----------|-------------|------------|
| Register User | O(1) | O(n) |
| Unregister User | O(1) | O(n) |
| Add New Movie | O(log n) | O(n) |
| Distribute Movies | O(n) | O(n) |
| Watch Movie | O(log n) | O(n) |
| Filter Movies | O(n log n) | O(n log n) |
| User Statistics | O(k) | O(k) |
| Search Movie | O(log n) | O(n) |

Where:
- n = number of movies in relevant structure
- k = number of movies watched by user
- Worst cases occur with degenerate trees (before distribution phase)

## Implementation Notes
- **Universal Hashing**: Parameters a and b are randomly generated
- **Prime Selection**: Binary search over predefined prime array
- **Sentinel Nodes**: movieID = -1 marks tree boundaries
- **LOBST Structure**: Parent pointers enable efficient leaf navigation
- **Heap Sort**: Used for stable sorting with custom comparison
- **Error Handling**: Comprehensive input validation and memory checks
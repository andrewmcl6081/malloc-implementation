#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)     ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr) - 1)

static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0;
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0;
static int num_requested     = 0;
static int max_heap          = 0;

//$ env LD_PRELOAD=lib/libmalloc-ff.so tests/ffnf

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
   printf("\nheap management statistics\n");
   printf("mallocs:\t%d\n", num_mallocs );
   printf("frees:\t\t%d\n", num_frees );
   printf("reuses:\t\t%d\n", num_reuses );
   printf("grows:\t\t%d\n", num_grows );
   printf("splits:\t\t%d\n", num_splits );
   printf("coalesces:\t%d\n", num_coalesces );
   printf("blocks:\t\t%d\n", num_blocks );
   printf("requested:\t%d\n", num_requested );
   printf("max heap:\t%d\n", max_heap );
}

struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *next;  /* Pointer to the next _block of allcated memory   */
   bool   free;          /* Is this _block free?                            */
   char   padding[3];    /* Padding: IENTRTMzMjAgU3ByaW5nIDIwMjM            */
};


struct _block *heapList = NULL; /* Free list to track the _blocks available */
struct _block *lastAlloc = NULL;

/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList; // setting curr to the beginning of the list

#if defined FIT && FIT == 0
   /* First fit */
   //
   // While we haven't run off the end of the linked list and
   // while the current node we point to isn't free or isn't big enough
   // then continue to iterate over the list.  This loop ends either
   // with curr pointing to NULL, meaning we've run to the end of the list
   // without finding a node or it ends pointing to a free node that has enough
   // space for the request.
   // 
   while (curr && !(curr->free && curr->size >= size)) // while the pointer is valid and  the curr node isnt free and isnt big enough
   {
      *last = curr; //keeps track of the last block of memory so we know where to tie the new one to when we hit a null pointer
      curr  = curr->next; // advance to the next block
   }
#endif

// \TODO Put your Best Fit code in this #ifdef block
#if defined BEST && BEST == 0
   
   struct _block* winner = NULL;
   int winningRemainder = INT_MAX;

   while(curr != NULL)
   {
      if(curr->free && curr->size >= size) //if the block is free and our requested size can fit
      {
         if(curr->size - size < winningRemainder) // if the remaining space is smaller than the max
         {
            winningRemainder = curr->size - size; // set winning remainder
            winner = curr; // update which block our winner is
         }
      }

      *last = curr; //keep track of last in case we never find a free block we can fit in
      curr = curr->next; // advance to next block
   }

   curr = winner; // set curr to winner to then be returned
#endif

// \TODO Put your Worst Fit code in this #ifdef block
#if defined WORST && WORST == 0
   
   struct _block* winner = NULL;
   int winningRemainder = 0;

   while(curr != NULL)
   {
      if(curr->free && curr->size >= size) //if the block is free and our requested size can fit
      {
         if(curr->size - size > winningRemainder) // if the remaining space is bigger than the minimum
         {
            winningRemainder = curr->size - size; // set winning remainder
            winner = curr; // update which block our winner is
         }
      }

      *last = curr;
      curr = curr->next; // advance to next block
   }

   curr = winner; // set curr to winner to then be returned
#endif

// \TODO Put your Next Fit code in this #ifdef block
#if defined NEXT && NEXT == 0

   curr = lastAlloc; // start searching at last allocation


   // while our pointer curr is valid and it is not free and not big enough
   while (curr && !(curr->free && curr->size >= size))
   {
      *last = curr; // keeps track of the last block of memory so we know where to tie a new block to if we need one
      curr  = curr->next; // advance to the next block
   }


   // if we find a spot, curr now holds that spot and the if block wont execute
   // if we didnt find a spot curr is null
   // last will now hold our last block if we found nothing


   // if we didnt find anything in the rest of our heap search the front half
   if(curr == NULL && heapList != NULL)
   {  
      // start at the beginning
      curr = heapList;

      // iterate while we arent at lastAlloc
      while(curr && (curr != lastAlloc))
      {
         // if block is free and big enough
         if(curr->free && (curr->size >= size))
         {
            // return this block
            return curr;
         }
         else
         {
            // advance and keep searching
            curr = curr->next;
         }
      }

      // we found no space in our heap
      return NULL;
   }

#endif

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0); // curr will somehow now have our new requested memory
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }
   else
   {
      // update statistics
      max_heap = max_heap + (int)size;
      num_blocks += 1;
      num_grows += 1;
   }

   /* Update heapList if not set */
   if (heapList == NULL) // if we have nothing in our heaplist linked list head yet
   {
      heapList = curr;
   }


   /* Attach new _block to previous _block */
   if (last) 
   {
      last->next = curr; //setting the current tail of our linked list to link to the new block
   }


   /* Update _block metadata:
      Set the size of the new block and initialize the new block to "free".
      Set its next pointer to NULL since it's now the tail of the linked list.
   */

   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{
   num_requested = num_requested + ((int)size);

   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }

   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0) 
   {
      return NULL;
   }

   /* Look for free _block.  If a free block isn't found then we need to grow our heap. */

   struct _block *last = heapList; //reseting last to the beginning of the list
   struct _block *next = findFreeBlock(&last, size); //next will be null or a head ptr to an available block



   // if current size - requested size is big enough for block size + minimum byte allocation requirement
   if(next && ( next->size - size >= ((size_t) ( sizeof(struct _block) + 4) )))
   {

      struct _block* next_block = next->next; // keeping track of the where the free block points to next
      size_t curr_size = next->size; // keep track of the unmodified size

      next->size = size; // setting the free block to have the new size;

      struct _block* temp_block = next; // temp ptr so next remains unmodified
      unsigned char* temp = (unsigned char*) BLOCK_DATA(temp_block); // converting the block header addreess to where the block data starts and then type casting
      temp = temp + (int)size; // moving the pointer to where our next block header will start
      

      next->next = (struct _block*) temp; // our current block will now point to the next free block header we just made
      next->next->size = curr_size - size - ((size_t)(sizeof(struct _block))); // setting size of new free block
      next->next->free = true; // new free block is free for use
      next->next->next = next_block;


      

      num_splits += 1;
      num_blocks += 1;
   }
   

   /* Could not find free _block, so grow heap */
   if (next == NULL) 
   {
      next = growHeap(last, size); // passing in last so we know where to tie the new allocation to
   }
   else
   {
      num_reuses += 1;
   }


   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL) 
   {
      return NULL;
   }
   else // successful malloc
   {
      num_mallocs += 1;
   }
   
   
   /* Mark _block as in use */
   next->free = false;

   lastAlloc = next;
   /* Return data address associated with _block to the user */
   return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   if (ptr == NULL) 
   {
      return;
   }
   else // points to a valid address
   {
      num_frees += 1;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;


   // Coalesce free blocks
   struct _block* curr_block = heapList;
   struct _block* next_block;
   
   if(curr_block != NULL)
   {
      while(curr_block->next != NULL)
      {
         next_block = curr_block->next;

         if(curr_block->free && next_block->free)
         {
            curr_block->size = curr_block->size + next_block->size + ((size_t)sizeof(struct _block));
            curr_block->next = next_block->next;


            // update statistics
            num_coalesces += 1;
            num_blocks -= 1;
         }
         else
         {
            curr_block = curr_block->next;
         }
      }
   }

   
   return;
}

void *calloc( size_t nmemb, size_t size )
{
   size_t space_requested = nmemb * size;

   void* ptr = malloc(space_requested);
   memset(ptr, 0, space_requested);

   return ptr;
}

void *realloc( void *ptr, size_t size )
{
   if(ptr == NULL)
   {
      return malloc(size);
   }
   else if( (ptr != NULL) && (size == ((size_t)0)) )
   {
      free(ptr);
      return NULL;      
   }
   
   void* new_ptr = malloc(size); //allocating a new block
   void* old_ptr = ptr; //pointer to old user data

   struct _block* old_block = (struct _block*) BLOCK_HEADER(ptr); // old block will be old block header ptr
   memcpy(new_ptr, old_ptr, old_block->size); // copying over users data
   free(ptr);
   


   return new_ptr;
}



/* vim: IENTRTMzMjAgU3ByaW5nIDIwMjM= -----------------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
/*
 *1) In the module entry point, create a linked list containing five struct birthday elements
 (respectively for the following people, from head to tail: Alice, Bob, Mallory, Nancy and
 Kate). Traverse the linked list and output its contents to the kernel log buffer. Invoke the
 dmesg command to ensure the list is properly constructed once the kernel module has been
 loaded. Please take the screen shots.
 Alice born on Jan 9, 1999;
 Bob born on March 8, 1978;
 Mallory born on Dec 9, 1958;
 Nancy born on Jun 9, 2003;
 Kate born on March 8, 1978;

 2) Please sort the five struct birthday elements in the list by this sequence: from head to tail, the people are from old to young. You may need to come up with new functions if you need. Traverse the linked list again and output its contents to the kernel log buffer. Invoke
 the dmesg command to ensure the list is properly rearranged. Please take the screen shots.
 
 3) In the module exit point, delete the elements from the linked list and return the free memory
 back to the kernel. Again, invoke the dmesg command to check that the list has been
 removed once the kernel module has been unloaded. Please take the screen shots.
 */


struct birthday {
  char* name;
  int day;
  int month;
  int year;
  struct list_head list;
};

struct birthday *person, *tmp, *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;
static LIST_HEAD(birthday_list);
struct list_head *pos;

/* This function is called when the module is loaded. */
int simple_init(void)
{
  
  //Alice first
  person = kmalloc(sizeof(*person),GFP_KERNEL);
  person->name = "Alice";
  person->day = 1;
  person->month = 9;
  person->year = 1999;

  char* bob_name = "Bob";
  int bob_day = 8;
  int bob_month = 3;
  int bob_year = 1978;
  
  char* mallory_name = "Mallory";
  int mallory_day = 9;
  int mallory_month = 12;
  int mallory_year = 1958;

  char* nancy_name = "Nancy";
  int nancy_day = 9;
  int nancy_month = 6;
  int nancy_year = 2003;

  char* kate_name = "Kate";
  int kate_day = 8;
  int kate_month = 3;
  int kate_year = 1978;
  
  ///set to head
  INIT_LIST_HEAD(&person->list);
  printk(KERN_INFO "Loading Module\n");

  ptr1 = kmalloc(sizeof(*ptr1), GFP_KERNEL);
  ptr1->name = bob_name;
  ptr1->day = bob_day;
  ptr1->month = bob_month;
  ptr1->year = bob_year;
  list_add(&(ptr1->list), &(person->list)); 

  ptr2 = kmalloc(sizeof(*ptr2), GFP_KERNEL);
  ptr2->name = mallory_name;
  ptr2->day = mallory_day;
  ptr2->month = mallory_month;
  ptr2->year = mallory_year;
  list_add(&(ptr2->list), &(person->list)); 

  ptr3 = kmalloc(sizeof(*ptr3), GFP_KERNEL);
  ptr3->name = nancy_name;
  ptr3->day = nancy_day;
  ptr3->month = nancy_month;
  ptr3->year = nancy_year;
  list_add(&(ptr3->list), &(person->list)); 

  ptr4 = kmalloc(sizeof(*ptr4), GFP_KERNEL);
  ptr4->name = kate_name;
  ptr4->day = kate_day;
  ptr4->month = kate_month;
  ptr4->year = kate_year;
  list_add(&(ptr4->list), &(person->list)); 

  /*
  ptr5 = kmalloc(sizeof(*ptr5), GFP_KERNEL);
  ptr5->day = nancy_day;
  ptr5->month = nancy_month;
  ptr5->year = nancy_year;
  list_add(&(ptr5->list), &(person->list)); 
  */

	list_for_each(pos, &(person->list)){
		tmp = list_entry(pos, struct birthday, list);
		printk("day = %d month = %d year = %d\n", tmp->day, tmp->month, tmp->year);
	}

	list_for_each(pos, &(person->list)
	{
		//tmpval = list_entry
	}


  return 0;



}

/*
 * sort
 */
int compare(const void *s1, const void *s2)
{
	struct birthday *e1 = (struct birthday *)s1;
	struct brithday *e2 = (struct birthday *)s2;
/*
	int yearCompare = strcmp(e1->gender, e2->gender);
	if (gendercompare == 0)  //same gender so sort by id
		return e1->id - e2->id;
	else
		return -gendercompare;
*/
	int yearCompare = 0;
	if (e1->year > e2->year)
	{
		yearCompare = 1;
	}
	if(e1->year < e2->year)
	{
		yearCompare = -1;
	}
	else if (e1->year == e2->year)
	{
	  int monthCompare = 0;
		if (e1->month > e2->month)
		{
			monthCompare = 1;
		}
		if(e1->month < e2->month)
		{
			monthCompare = -1;
		}
		else if (e1->month == e2->month)
		{
			int dayCompare = 0;
			if (e1->day > e2->day)
			{
				dayCompare = 1;
			}
			
			if(e1->day < e2->day)
			{
				dayCompare = -1;
			}
			else if (e1->day == e2->day)
			{
				yearCompare = -2;		
			}
		}
	}

}


/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");


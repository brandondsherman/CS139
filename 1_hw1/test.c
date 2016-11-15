#include <linux/init.h>  
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/random.h>

struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;
};

struct birthday *my_bday, *tmp, *ptr;
struct list_head *pos;

int simple_init(void)
{
    int i, day,month,year;
    my_bday = kmalloc(sizeof(*my_bday), GFP_KERNEL);
    my_bday->day = 30;
    my_bday->month=5;
    my_bday->year=1984;

    /* set my_bday to head of list*/

    INIT_LIST_HEAD(&(my_bday->list));
    printk(KERN_INFO "Loading Module\n");
    
		for(i=0; i<5; ++i){
			ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);    
			//random birthday variables
			get_random_bytes(&day, 1);
			ptr->day = day % 31;
			if(ptr->day < 0)
					ptr->day = ptr->day * -1;

			get_random_bytes(&month, 1);
			ptr->month = month % 12;

			if(ptr->month < 0)
					ptr->month = ptr->month * -1;

			get_random_bytes(&year, 1);
			ptr->year = (year % 2000)+1900;

			if(ptr->year < 0)
					ptr->year = ptr->year * -1;

			list_add(&(ptr->list), &(my_bday->list));
    }

    printk(KERN_INFO "traversing the list using list_for_each()\n");
    list_for_each(pos, &(my_bday->list)){
        tmp = list_entry(pos, struct birthday, list);
        printk("day = %d month = %d year = %d\n", tmp->day, tmp->month, tmp->year);
    }
    return 0;
}

/*
void simple_exit(void)
{
    printk(KERN_INFO "Removing Module\n");  
    list_for_each(pos, &(my_bday->list)){
        tmp = list_entry(pos, struct birthday, list);
        list_del(&tmp->list);
        kfree(tmp);
    }
}
*/

void simple_exit(void)
{
    printk(KERN_INFO "Removing Module\n");  

    list_for_each_entry_safe(ptr, tmp, &my_bday->list, list) {
        list_del(&(ptr->list));
        kfree(ptr);
    }
}

module_init(simple_init);

module_exit(simple_exit);


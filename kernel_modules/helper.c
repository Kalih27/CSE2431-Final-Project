/********* helper.c ***********/

/*
	struct Process
		-burst_time: given, time it takes for the process to complete
		-next:      generated, next process in queue
		-previous:  generated, process before self in queue
*/
struct Process
{
	int burst_time;
	pid_t proc_pid;
	struct Process *next;
	struct Process *previous;
};

/*
	static int check_alloc()
		-checks if the given pointer has been allocated successfully and exits if it did not
*/
static int check_alloc(void *ptr)
{
	if (!ptr)
	{
		printk("Allocation error!\n");
		return 0;
	}
	return 1;
}

/*
	struct Process* create_new_process(int process_burst_time, pid_t pid)
		-creates a pointer to a Process struct
*/
struct Process *create_new_process(int process_burst_time, pid_t pid)
{
	int ret = 0;
	struct Process *new_process = NULL;

	new_process = (struct Process *)kmalloc(sizeof(struct Process), GFP_KERNEL);
	if (check_alloc(new_process) == 0)
	{
		pr_err("An error occurred during initialization\n");
		return -EINVAL; // Return "Invalid argument" error
	}

	new_process->burst_time = process_burst_time;
	new_process->next = NULL;
	new_process->proc_pid = pid;
	new_process->previous = NULL;

	return new_process;
}

unsigned long virtu_to_phys(struct mm_struct *mm, unsigned long vpage)
{
	unsigned long physical_page_addr;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	struct page *page;
	pgd = pgd_offset(mm, vpage);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		return 0;
	p4d = p4d_offset(pgd, vpage);
	if (p4d_none(*p4d) || p4d_bad(*p4d))
		return 0;
	pud = pud_offset(p4d, vpage);
	if (pud_none(*pud) || pud_bad(*pud))
		return 0;
	pmd = pmd_offset(pud, vpage);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		return 0;
	if (!(pte = pte_offset_map(pmd, vpage)))
		return 0;
	if (!(page = pte_page(*pte)))
		return 0;
	physical_page_addr = page_to_phys(page);
	pte_unmap(pte);
	// handle unmapped pages
	if (physical_page_addr == 70368744173568)
		return 0;
	return physical_page_addr;
}

int proc_count(void)
{
	int i = 0, counter = 0, contig_counter = 0, non_contig_counter = 0;
	struct task_struct *task;
	struct vm_area_struct *vma = 0;
	unsigned long vpage;

	printk("PROCESS REPORT:\n");
	printk("proc_id,proc_name,contig_pages,non_contig_pages,total_pages\n");

	for_each_process(task)
	{
		int total_pages = 0;
		int contig = 0, non_contig = 0;
		if (task->pid > 650)
		{
			if (task->mm && task->mm->mmap)
			{
				unsigned long previous_page_addr;
				for (vma = task->mm->mmap; vma; vma = vma->vm_next)
				{
					for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE)
					{
						unsigned long physical_page_addr = virtu_to_phys(task->mm, vpage);
						if (physical_page_addr != 0)
						{
							if (physical_page_addr == previous_page_addr + PAGE_SIZE)
							{
								contig++;
							}
							else
							{
								non_contig++;
							}
							previous_page_addr = physical_page_addr;
							total_pages++;
						}
					}
				}
			}
			counter += total_pages;
			printk("%d,%s,%d,%d,%d\n", task->pid, task->comm, contig, non_contig, total_pages);
			i++;
		}
		contig_counter += contig;
		non_contig_counter += non_contig;
	}

	printk("TOTALS,%d,%d,%d\n", contig_counter, non_contig_counter, counter);

	return i;
}
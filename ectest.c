#include <ecrt.h> // The ethercat lib
#include <stdio.h>
#include <time.h>
#include <sys/ioctl.h>

#define FREQUENCY 1000
#define CLOCK_TO_USE CLOCK_MONOTONIC

#define NSEC_PER_SEC (1000000000L)
#define PERIOD_NS (NSEC_PER_SEC / FREQUENCY)

#define TIMESPEC2NS(T) ((uint64_t) (T).tv_sec * NSEC_PER_SEC + (T).tv_nsec)

#define SlaveAlias 69
#define SlavePos 0
#define SlaveVendorId 0x0000079A
#define SlaveProductCode 0xDEADBEEF

// Master pointer and state
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

// Domain (data exchange area) pointer and state
static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

const struct timespec cycletime = {0, PERIOD_NS};

// Process data
static uint8_t *domain1_pd = NULL; // To access data in the domain and send data in the domain. Essentially the pointer to the start of the domain.
static unsigned int receiveOffset; // Offset from the start of the domain, to access the correct byte.
static unsigned int sendOffset; // Offset from the start of the domain, to access the correct byte.

static const ec_pdo_entry_reg_t domain1_regs[] = // Register the RX_pdos (data to receive)
		{{SlaveAlias, SlavePos, SlaveVendorId, SlaveProductCode, 0x0005, 1, &receiveOffset, 0}}; // One variable to receive per line.

// Helper function for timespecs
struct timespec timespec_add(struct timespec time1, struct timespec time2);

void check_master_state(void)
{
	// TODO: check if it is necessary (directly use master_state otherwise)
    ec_master_state_t ms;

    ecrt_master_state(master, &ms); // Read the current master state

    if (ms.slaves_responding != master_state.slaves_responding)
        printf("%u slave(s).\n", ms.slaves_responding);
    if (ms.al_states != master_state.al_states)
        printf("AL states: 0x%02X.\n", ms.al_states);
    if (ms.link_up != master_state.link_up)
        printf("Link is %s.\n", ms.link_up ? "up" : "down");

    master_state = ms;
}

void check_domain1_state(void)
{
	// TODO: check if it is necessary (directly use domain1_state otherwise)
    ec_domain_state_t ds;

    ecrt_domain_state(domain1, &ds);

    if (ds.working_counter != domain1_state.working_counter)
        printf("Domain1: WC %u.\n", ds.working_counter);
    if (ds.wc_state != domain1_state.wc_state)
        printf("Domain1: State %u.\n", ds.wc_state);

    domain1_state = ds;
}

void cyclic_task()
{
	struct timespec wakeupTime, time;
	clock_gettime(CLOCK_TO_USE, &wakeupTime);
	
	int count = 0;
	while(1)
	{
		// Debug count for testing
		count ++;
		count = count%10;
		// Define the cycling period
		// TODO: make it clearer
		wakeupTime = timespec_add(wakeupTime, cycletime);
        clock_nanosleep(CLOCK_TO_USE, TIMER_ABSTIME, &wakeupTime, NULL);

		// Set the application time
		ecrt_master_application_time(master, TIMESPEC2NS(wakeupTime));
		
		// Fetch data
		ecrt_master_receive(master);
		//ecrt_domain_process(domain1);
		//check_domain1_state();

		printf("data: %u\n", EC_READ_U8(domain1_pd + receiveOffset));
		EC_WRITE_U8(domain1_pd + sendOffset, count);

		// Enqueue data from EC_WRITE_..
		ecrt_domain_queue(domain1);

		// Actually send data
		ecrt_master_send(master);
	}
}

// TODO: init function with all this then only cyclic task
int main ( void )
{
	
	ec_slave_config_t *sc;
	// Get master
	master = ecrt_request_master (0) ;
	if (! master )
	{
		printf("Master NOT found\n");
		return 1; // error
	}
	else
	{
		printf("Master found\n");
	}
	check_master_state();

	// Create domain
	domain1 = ecrt_master_create_domain(master);
    if (!domain1)
	{
		printf("Domain1 could not be created\n");
		return -1;
	}
	else
	{
		printf("Domain1 created\n");
	}

    // Create receive slave config (obtain a slave configuration)
    sc = ecrt_master_slave_config(master, SlaveAlias, SlavePos, SlaveVendorId, SlaveProductCode);
    if (!sc)
	{
		printf("Could not ecrt_master_slave_config\n");
        return -1;
	}
	else
	{
		printf("master slave config created\n");
	}

	// Register PDOs entry for exchange in domain
	
    receiveOffset = ecrt_slave_config_reg_pdo_entry(sc, 0x0006, 1, domain1, NULL);
    if (receiveOffset < 0)
	{
		printf("Could not register PDO\n");
        return -1;
	}
	else
	{
		printf("PDO registered\n");
	}
	
    sendOffset = ecrt_slave_config_reg_pdo_entry(sc, 0x0005, 1, domain1, NULL);
    if (sendOffset < 0)
	{
		printf("Could not register PDO\n");
        return -1;
	}
	else
	{
		printf("PDO registered\n");
	}
	
	// Activating
	printf("Activating master...\n");
    if (ecrt_master_activate(master))
	{
		printf("Could not activate master\n");
		return -1;
	}
	else
	{
		printf("Master activated\n");
	}

	if (!(domain1_pd = ecrt_domain_data(domain1))) // Get the domain's process data. Has to be called after ecrt_master_actiate
	{
		printf("Could not ecrt_domain_data(domain1)\n");
        return -1;
    }
	else
	{
		printf("Got domain's process data\n");
	}

	printf("Starting cyclic function.\n");
    cyclic_task();
	
	return 0;
}

struct timespec timespec_add(struct timespec time1, struct timespec time2)
{
    struct timespec result;

    if ((time1.tv_nsec + time2.tv_nsec) >= NSEC_PER_SEC) {
        result.tv_sec = time1.tv_sec + time2.tv_sec + 1;
        result.tv_nsec = time1.tv_nsec + time2.tv_nsec - NSEC_PER_SEC;
    } else {
        result.tv_sec = time1.tv_sec + time2.tv_sec;
        result.tv_nsec = time1.tv_nsec + time2.tv_nsec;
    }

    return result;
}
#include <stdio.h>

int main(int argc, char *argv[])
{ // The usual start of a C program
    int distance = 100;
    float power = 2.345f;
    double super_power = 56789.4532;
    char initial = 'A';
    char first_name[] = "Zed";
    char last_name[] = "Shaw";
    
    //first_name[3] = "x";

    printf("You are %d miles away.\n", distance);
    printf("You have %f leverls of power.\n", power);
    printf("You have %f awesome super power.\n", super_power);
    printf("I have an initial %c.\n", initial);
    printf("I have a first name %s.\n", first_name);
    printf("I have a last name %s.\n", last_name);
    printf("My whole name is %s %c. %s.\n", 
            first_name, initial, last_name);
    // Declare an int and double for some fake bug data
    int bugs = 100;
    double bug_rate = 1.2;
    
    // Print out those two, so nothing new here.
    printf("You have %d bugs at the imaginary rate of %f.\n",
            bugs, bug_rate);
    
    // Declare a huge number using a new type, long, for storing big numbers.
    // 这里顺便说一句，我试过把long 换成 int,还是可以运行
    long universe_of_defects = 1 * 1024L * 1024L * 1024L;
    //Print out that number using %ld that adds a modifier to the usual %d.
    //Adding l (the letter) tells the program to print the number as a long decimal.
    printf("The entire universe has %ld bugs.\n", universe_of_defects);

    double expected_bugs = bugs * bug_rate;
    printf("You are expected to have %f bugs.\n", expected_bugs);
    /*
        Craft a depiction of your bug rate compared to the bugs in the
      universe, which is a completely inaccurate calculation. It's so small
      that we have to use %e to print it in scientific notation.
    */
    double part_of_universe = expected_bugs / universe_of_defects;
    printf("That is only a %e portion of the universe.\n",
            part_of_universe);
    
    // this makes no sense, just a demo of something weird
    //Make a character, with a special syntax '\0' that creates a nul byte character. 
    //This is effectively the number 0.
    char nul_byte = '\0';
    int care_percentage = bugs * nul_byte;
    //Print that out, and notice we've used %%(two percent signs) so that we can print a % character.
    printf("Which means you should care %d%%.\n", care_percentage);

    return 0;// The end of the main function.
}
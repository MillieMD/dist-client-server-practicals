/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

unsigned int Generate_Checksum(const char *filename){
    FILE* file = fopen(filename, "r");

    if(!file){
        printf("File '%s' not found", filename);
        exit(1);
    }
    
    unsigned int checksum = 0;
    
    char ch = fgetc(file);
    while(!feof(file)){
        checksum += ch;
        ch = fgetc(file);
    }
    
    return checksum;
}

/* Characters are represented by a "byte" - which can be converted to 
 a decimal value e.g A = 65, B = 66. 
 These values are summed together to create the checksum. 
 Changing a single value, will change the final sum, flagging as a corrupted file.
 It is possible to avoid this simple checksum by changing characters in a way 
 that cancels out - e.g. increase one char by 1, decrease another by 1.
 Example:
 "3.1" -> "3.2" breaks the checksum
 but "3.1" -> "2.2" doesn't break the checksum
 
 Real systems use a method called hashing where more complex algorithms are used.
 These algorithms always generate the same answer for a particular file, 
 and can change drastically if even a small change is made in the original file. 
 Making them less succeptible to the above examples.
 
*/
int Verify_Integrity(const char *filename, unsigned int expected_tag){
    unsigned int tag = Generate_Checksum(filename);
    
    return(expected_tag == tag);
}

int main()
{
    // Saved from activity 1, otherwise the program will always pass as key is regenned
    unsigned int original_tag = 3893;
    
    if(Verify_Integrity("software_package.txt", original_tag)){
        printf("SECURITY CHECK PASSED: File is authentic\n");
    }else{
        printf("SECURITY ALERT: FIle has been TAMPERED or CORRUPTED!\n");
        printf("Installation blocked. Contact administrator\n");
    }
    
    return 0;
}
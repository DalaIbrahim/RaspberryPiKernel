#include <kernel/mem.h>
#include <kernel/atag.h>
#include <kernel/uart.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <stdarg.h> // For va_list, va_start, va_arg, va_end

// Function prototypes
int custom_strcmp(const char *str1, const char *str2);
int custom_atoi(const char *str);
void custom_printf(const char *format, ...);
void encrypt(char *str, int shift);
void decrypt(char *str, int shift);
char *custom_strchr(const char *str, int c); // Moved prototype here

// LinkedList function prototypes
typedef struct Node Node;
Node* create_node(int data);
void add_node(int data);
void display_list();
void clear_list();

// LinkedList node structure
struct Node {
    int data;
    struct Node* next;
};

Node* head = NULL;  // Head of the linked list

// Simplified memory allocation functions 
void* simple_heap = (void*)0x10000;  
size_t heap_size = 0x10000;  

void* simplified_kmalloc(uint32_t size) { 
    if (size > heap_size) { 
        puts("Out of memory\n"); 
        return NULL; 
    } 
    void* allocated_memory = simple_heap;  
    simple_heap += size;  
    heap_size -= size;  
    return allocated_memory;  
}

void simplified_kfree(void* ptr) {  
    (void)ptr;  
} 

// LinkedList functions
Node* create_node(int data) {
    Node* new_node = (Node*)simplified_kmalloc(sizeof(Node));
    if (new_node == NULL) {
        puts("Failed to create a new node\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void add_node(int data) {
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        puts("Failed to create a new node\n");
        return;
    }
    if (head == NULL) {
        head = new_node;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void display_list() {
    if (head == NULL) {
        puts("List is empty\n");
        return;
    }
    Node* current = head;
    while (current != NULL) {
        custom_printf("%d -> ", current->data);
        current = current->next;
    }
    puts("NULL\n");
}

void clear_list(void) {
    Node *current = head;
    Node *next_node;
    while (current) {
        next_node = current->next;
        simplified_kfree(current);
        current = next_node;
    }
    head = NULL;
}

// Implement the utility functions
int custom_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int custom_atoi(const char *str) {
    int result = 0;
    int sign = 1;

    // Check for empty string
    if (*str == '\0') {
        return 0;
    }

    // Check if the first character is a negative sign
    if (*str == '-') {
        sign = -1;
        str++;
    }

    // Iterate through each character of the string
    while (*str) {
        // Check if the character is a digit
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        } else {
            // If a non-digit character is found, break the loop
            break;
        }
        str++;
    }

    // Return the result multiplied by the sign
    return result * sign;
}

void custom_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%' && *(format + 1) != '\0') {
            format++;
            if (*format == 's') {
                char *str_arg = va_arg(args, char*);
                while (*str_arg) {
                    uart_putc(*str_arg++);
                }
            } else if (*format == 'd') {
                int int_arg = va_arg(args, int);
                char buffer[20]; // Buffer to hold the converted string
                char *p = buffer;
                int num_chars = 0;

                // Handle zero explicitly
                if (int_arg == 0) {
                    uart_putc('0');
                    format++;
                    continue;
                }

                // Handle negative numbers
                if (int_arg < 0) {
                    uart_putc('-');
                    int_arg = -int_arg;
                }

                // Convert integer to string
                while (int_arg > 0) {
                    int digit = int_arg % 10;
                    *p++ = digit + '0';
                    int_arg /= 10;
                    num_chars++;
                }

                // Print the number in reverse order
                while (num_chars > 0) {
                    uart_putc(*--p);
                    num_chars--;
                }
            }
        } else {
            uart_putc(*format);
        }
        format++;
    }

    va_end(args);
}

void encrypt(char *str, int shift) {
    char *ptr = str;
    while (*ptr) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = ((*ptr - 'a' + shift) % 26 + 26) % 26 + 'a'; // Ensure positive shift
        } else if (*ptr >= 'A' && *ptr <= 'Z') {
            *ptr = ((*ptr - 'A' + shift) % 26 + 26) % 26 + 'A'; // Ensure positive shift
        }
        ptr++;
    }
}

void decrypt(char *str, int shift) {
    encrypt(str, -shift); // Decryption is just encryption with negative shift
}

char *custom_strchr(const char *str, int c) { // Moved implementation here
    while (*str) {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }
    return NULL;
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_putc('H'); // Send single character to test UART output
    uart_putc('e');
    uart_putc('l');
    uart_putc('l');
    uart_putc('o');
    uart_putc('\n'); // Newline character

    custom_printf("Initializing Memory Module\n");
    mem_init((atag_t *)atags);

    custom_printf("CMPS240 Project Summer 2024!\n");

    // Example usage of implemented functions
    custom_printf("Welcome to CMPS 240 Custom Kernel!\n");

    char buf[256];
    while (1) {
        custom_printf("> ");
        gets(buf, 256); // Ensure this function is correctly implemented

        // Remove newline character if present
        char *newline = custom_strchr(buf, '\n');
        if (newline) {
            *newline = '\0';
        }

        if (custom_strcmp(buf, "help") == 0) {
            custom_printf("Available commands:\n");
            custom_printf("help: Display this help message\n");
            custom_printf("sum: Calculate the sum of two integers\n");
            custom_printf("encrypt: Encrypt a message with a Caesar cipher\n");
            custom_printf("decrypt: Decrypt a message with a Caesar cipher\n");
            custom_printf("addnode: Add a node to the linked list\n");
            custom_printf("displaylist: Display the linked list\n");
            custom_printf("clearlist: Clear the linked list\n");
            custom_printf("exit: Exit the program\n");
        } else if (custom_strcmp(buf, "sum") == 0) {
            custom_printf("Enter first integer: ");
            gets(buf, 256);
            int num1 = custom_atoi(buf);

            custom_printf("Enter second integer: ");
            gets(buf, 256);
            int num2 = custom_atoi(buf);

            custom_printf("The sum is: %d\n", num1 + num2);
        } else if (custom_strcmp(buf, "encrypt") == 0) {
            char message[256];
            char shift_str[256];
            int shift;
            custom_printf("Enter shift value: ");
            gets(shift_str, 256);
            shift = custom_atoi(shift_str);
            custom_printf("Enter message to encrypt: ");
            gets(message, 256);
            encrypt(message, shift);
            custom_printf("Encrypted message: %s\n", message);
        } else if (custom_strcmp(buf, "decrypt") == 0) {
            char message[256];
            char shift_str[256];
            int shift;
            custom_printf("Enter shift value: ");
            gets(shift_str, 256);
            shift = custom_atoi(shift_str);
            custom_printf("Enter message to decrypt: ");
            gets(message, 256);
            decrypt(message, shift);
            custom_printf("Decrypted message: %s\n", message);
        } else if (custom_strcmp(buf, "addnode") == 0) {
            char data_str[256];
            int data;
            custom_printf("Enter integer to add to the linked list: ");
            gets(data_str, 256);
            data = custom_atoi(data_str);
            add_node(data);
        } else if (custom_strcmp(buf, "displaylist") == 0) {
            display_list();
        } else if (custom_strcmp(buf, "clearlist") == 0) {
            clear_list();
        } else if (custom_strcmp(buf, "exit") == 0) {
            break;
        } else {
            custom_printf("Unknown command. Type 'help' for a list of commands.\n");
        }
    }
}

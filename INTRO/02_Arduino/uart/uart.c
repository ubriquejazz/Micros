#define UART_BUFFER_SIZE 64
#define BAUD_RATE 9600
#define BIT_PERIOD (1000000 / BAUD_RATE)  // Bit period in microseconds

volatile char tx_buffer[UART_BUFFER_SIZE];
volatile char rx_buffer[UART_BUFFER_SIZE];
volatile uint8_t tx_head = 0, tx_tail = 0;
volatile uint8_t rx_head = 0, rx_tail = 0;

volatile uint8_t rx_busy = 0;  // Flag to indicate ongoing reception
volatile uint8_t rx_bit_index = 0;
volatile char rx_byte = 0;

void TIMER_RX_ISR() {
    static uint8_t bit_position = 0;

    if (rx_bit_index == 0) {
        // Sample start bit
        if (READ_GPIO_RX() != 0) {
            // False start, reset RX
            rx_busy = 0;
            DISABLE_TIMER();
            return;
        }
    } else if (rx_bit_index <= 8) {
        // Sample data bits
        if (READ_GPIO_RX()) {
            rx_byte |= (1 << (rx_bit_index - 1));
        }
    } else if (rx_bit_index == 9) {
        // Sample stop bit
        if (READ_GPIO_RX() == 0) {
            // Stop bit error
            rx_busy = 0;
            DISABLE_TIMER();
            return;
        }

        // Store received byte in buffer
        uint8_t next_head = (rx_head + 1) % UART_BUFFER_SIZE;
        if (next_head != rx_tail) {
            rx_buffer[rx_head] = rx_byte;
            rx_head = next_head;
        }

        rx_busy = 0;
        DISABLE_TIMER();
        return;
    }

    rx_bit_index++;
}

void uart_init() {
    // Configure RX pin as input with falling edge interrupt (start bit detection)
    CONFIGURE_GPIO_RX_INTERRUPT();

    // Configure Timer for bit timing
    CONFIGURE_TIMER(BIT_PERIOD, TIMER_RX_ISR);
}

char uart_receive() {
    while (rx_head == rx_tail);  // Wait if buffer is empty

    char data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % UART_BUFFER_SIZE;
    return data;
}

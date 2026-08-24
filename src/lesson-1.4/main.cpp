#include <Arduino.h>

#define BUTTON_IN 16  // Кнопка підключена до GPIO 16

volatile uint32_t button_counter2 = 0;  // volatile обов'язковий для змінних, які змінюються всередині ISR

uint32_t last_count1 = 0; // Змінна для відслідковування змін у головному циклі

// Швидка функція обробки переривань (ISR), яка розміщується в оперативній пам'яті (IRAM)
void IRAM_ATTR button_isr1() {
  button_counter2++;
}

void setup() {
  pinMode(BUTTON_IN, INPUT_PULLUP);
  Serial.begin(115200);

  // Прив'язка переривання. FALLING означає спрацьовування при натисканні (перехід з 1 в 0)
  attachInterrupt(digitalPinToInterrupt(BUTTON_IN), button_isr1, FALLING);
}

void loop() {
  if (button_counter2 != last_count1) {
    last_count1 = button_counter2;
    Serial.print("Button Pressed! Count: ");
    Serial.println(last_count1);
  }

  delay(10);
}
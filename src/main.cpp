#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include <OneButton.h>

// --- Definición de Pines ---
// NOTA: El GPIO 3 en el S3 es una "Strapping Pin". Si tienes problemas al 
// subir código o bootear, asegúrate de NO tener el botón presionado al encender.
#define PIN_BTN1 17
#define PIN_BTN2 18
#define PIN_BTN3 16
#define PIN_BTN4 8
#define PIN_BTN5 3

// Instancias de HID para ESP32-S3
// GRUPO 2 (Nuevos - Pines Seguros)
#define PIN_BTN6 4  // Alt + G + N
#define PIN_BTN7 5  // Abrir Fortnite
#define PIN_BTN8 6  // Ctrl + C
#define PIN_BTN9 7  // Ctrl + V
#define PIN_BTN10 15 // Ctrl + X

USBHIDKeyboard Keyboard;
USBHIDConsumerControl Consumer;

// Instancias de botones (true = PullUp interno, activa con GND)
OneButton btn1(PIN_BTN1, true);
OneButton btn2(PIN_BTN2, true);
OneButton btn3(PIN_BTN3, true);
OneButton btn4(PIN_BTN4, true);
OneButton btn5(PIN_BTN5, true);
OneButton btn6(PIN_BTN6, true);
OneButton btn7(PIN_BTN7, true);
OneButton btn8(PIN_BTN8, true);
OneButton btn9(PIN_BTN9, true);
OneButton btn10(PIN_BTN10, true);

// Código RAW para tecla < en español
const uint8_t KEY_ISO_LESS = 100; 

// --- Declaración de Funciones ---
void abrirTerminalAdmin();
void escribirSimboloMenor();
void escribirSimboloMayor();
void abrirFortnite();
void abrirplaylist();
void abrirexe();

void setup() {
  // Inicialización USB
  Keyboard.begin();
  Consumer.begin();
  USB.begin();

  // --- CONFIGURACIÓN BOTONES 1-5 ---
  btn1.attachClick([]() { Consumer.press(CONSUMER_CONTROL_PLAY_PAUSE); Consumer.release(); });
  
  btn2.attachClick([]() { Consumer.press(CONSUMER_CONTROL_VOLUME_INCREMENT); Consumer.release(); });
  btn2.attachLongPressStart([]() { Consumer.press(CONSUMER_CONTROL_SCAN_NEXT); Consumer.release(); });
  
  btn3.attachClick([]() { Consumer.press(CONSUMER_CONTROL_VOLUME_DECREMENT); Consumer.release(); });
  btn3.attachLongPressStart([]() { Consumer.press(CONSUMER_CONTROL_SCAN_PREVIOUS); Consumer.release(); });
  
  btn4.attachClick(abrirTerminalAdmin);
  
  btn5.attachClick(escribirSimboloMenor);
  btn5.attachDoubleClick(escribirSimboloMayor);

  // --- CONFIGURACIÓN BOTONES 6-10 ---

  // Botón 6 (Pin 4): Alt + G + N
  btn6.attachClick([]() {
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('m');
    delay(100);
    Keyboard.releaseAll();
  });

  // Botón 7 (Pin 5): Abrir Fortnite
  btn7.attachDoubleClick(abrirFortnite);

  // Botón 8 (Pin 6): Ctrl + C (Copiar)
  btn8.attachClick(abrirexe);
  // Botón 9 (Pin 7): Ctrl + V (Pegar)
  btn9.attachClick([]() {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    delay(50);
    Keyboard.releaseAll();
  });
  btn9.attachDoubleClick([]() {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    delay(50);
    Keyboard.releaseAll();
  });

  //spotify
  btn10.attachClick(abrirplaylist);

}


void loop() {
  // Vigilamos los 10 botones constantemente
  btn1.tick(); btn2.tick(); btn3.tick(); btn4.tick(); btn5.tick();
  btn6.tick(); btn7.tick(); btn8.tick(); btn9.tick(); btn10.tick();
}

// --- IMPLEMENTACIÓN DE FUNCIONES ---

void abrirTerminalAdmin() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(300); // Espera ventana ejecutar
  
  Keyboard.print("cmd");
  delay(100);
  
  // Ctrl + Shift + Enter
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.releaseAll();
  
  // Esperar ventana UAC (Ajustar si es necesario)
  delay(1500); 
  
  // Aceptar permiso
  Keyboard.press(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.release(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
}

void escribirSimboloMenor() {
  KeyReport report = {0};
  report.keys[0] = 100; // Tecla fisica <
  report.modifiers = 0; 
  Keyboard.sendReport(&report);
  delay(50);
  Keyboard.releaseAll();
}

void escribirSimboloMayor() {
  KeyReport report = {0};
  report.keys[0] = 100; // Tecla fisica <
  report.modifiers = 2; // Shift Izquierdo
  Keyboard.sendReport(&report);
  delay(50);
  Keyboard.releaseAll();
}

void abrirFortnite() {
  // 1. Abrir ventana Ejecutar (Win + 
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(400); // Esperar a que abra la ventana

  // 2. Escribir "com.epicgames.launcher"
  Keyboard.print("com.epicgames.launcher");

  // 3. Escribir ":" (Shift + .)
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('.'); 
  delay(10);
  Keyboard.releaseAll();

  // 4. Escribir "//" (Shift + 7, dos veces)
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7'); 
  delay(10); Keyboard.release('7'); // Soltar solo el 7
  delay(10); Keyboard.press('7');   // Pulsar 7 otra vez
  delay(10); Keyboard.releaseAll(); // Soltar todo

  // 5. Escribir "apps"
  Keyboard.print("apps");

  // 6. Escribir "/" (Shift + 7)
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7');
  delay(10);
  Keyboard.releaseAll();

  // 7. Escribir "Fortnite"
  Keyboard.print("Fortnite");

  // 8. Escribir "?" (Shift + Tecla al lado del 0)
  // El código 0x2D es la tecla física que está a la derecha del 0.
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(0x2D); 
  delay(10);
  Keyboard.releaseAll();

  // 9. Escribir "action"
  Keyboard.print("action");

  // 10. Escribir "=" (Shift + 0)
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('0');
  delay(10);
  Keyboard.releaseAll();

  // 11. Escribir "launch"
  Keyboard.print("launch");

  // 12. Presionar Enter para ejecutar
  delay(50);
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
}
void abrirplaylist() {
  // Abrir Spotify (Win + R, escribir spotify, Enter)
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(400); // Esperar a que abra la ventana

  Keyboard.print("spotify");
  delay(100);

  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('.');
  delay(10); Keyboard.releaseAll();
  Keyboard.print("playlist");
  
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('.'); 
  delay(10); Keyboard.releaseAll();
  Keyboard.print("4othwhpwVEVJ601naENh1v");
  delay(50);
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
  delay(5000); 

  // 6. Presionar TAB una vez
  // Esto mueve el "foco" de Windows al botón verde gigante de "Play"
  Keyboard.press(KEY_TAB);
  delay(100);
  Keyboard.releaseAll();
  delay(50);
  Keyboard.press(KEY_TAB);
  delay(100);
  Keyboard.releaseAll();
  delay(50);
  Keyboard.press(KEY_TAB);
  delay(100);
  Keyboard.releaseAll();
  delay(50);
  Keyboard.press(KEY_TAB);
  delay(100);
  Keyboard.releaseAll();

  // 7. Presionar Enter
  // Esto "hace clic" en el botón verde
  delay(50);
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
}
void abrirexe () {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(400); 
  Keyboard.print("C");
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('.');
  delay(10);
  Keyboard.releaseAll();
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7'); 
  delay(10); Keyboard.release('7'); 
  delay(10); Keyboard.press('7');   
  delay(10); Keyboard.releaseAll();
  Keyboard.print("Users");
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7'); 
  delay(10); Keyboard.release('7'); 
  delay(10); Keyboard.press('7');   
  delay(10); Keyboard.releaseAll();
  Keyboard.print("sebo");
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7'); 
  delay(10); Keyboard.release('7'); 
  delay(10); Keyboard.press('7');   
  delay(10); Keyboard.releaseAll();
  Keyboard.print("Desktop");
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('7'); 
  delay(10); Keyboard.release('7'); 
  delay(10); Keyboard.press('7');   
  delay(10); Keyboard.releaseAll();
  Keyboard.print("app");
  Keyboard.press(KEY_RIGHT_SHIFT);
  Keyboard.press('?');
  Keyboard.releaseAll();
  Keyboard.print("luces.exe");
  Keyboard.press(KEY_RETURN);
  delay(50);
  Keyboard.releaseAll();
}

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <conio.h>  // Para _kbhit() y _getch()
#include <limits>   // Para numeric_limits
#include <random>   // Necesario para std::random_device y std::mt19937
// Incluir mmsystem.h para PlaySound
#include <mmsystem.h> // NeBcesario para PlaySound
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib") // Enlaza con la librería winmm.lib
#endif


using namespace std;
// ====== FUNCION GOTOXY PARA CENTRAR TEXTO ======
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// ====== FUNCION PARA SETEAR COLOR ======
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// ====== LOGO UNITORTE ======
string logo[] = {
    "..................................:+???+?+?+??+?+,..............................",
    "............................:++~.............+????+............................",
    "........................=+=.....................~????,..........................",
    ".....................+?=.........................+??++..........................",
    "..................~++......$=....................+???,..........................",
    ".ZZZZ.....ZZZ...+?=.......ZZZ..$ZZZ.....ZZZ$....~???~............?..............",
    "..ZZZ.....ZZZ..................$ZZZZ....ZZ.....................ZZZ..............",
    "..ZZZ.....ZZZ..ZZ.OZZZ7..ZZZZ..$ZZZZZ...ZZ....ZZZZZ7...$Z+?Z$.ZZZZZZ...~ZZZZZ...",
    "..ZZZ.....ZZZ..$ZZZ=ZZZZ.IZZZ..$ZZ.ZZZ..ZZ..7ZZZ.:ZZZ..ZZZZO$.7ZZZ$$..ZZZ..ZZZ..",
    "..ZZZ.....ZZ$..ZZZ...ZZZ..$ZZ..$ZZ..ZZZ.ZZ..ZZZ....ZZ$.ZZZ.....ZZZ...:ZZ....ZZ=.",
    "..ZZZ.....ZZZ..ZZ$...ZZZ..$ZZ..$ZZ..:ZZ=ZZ..ZZ=....ZZZ.ZZZ.....ZZZ...ZZZZZZZZZI.",
    "..+ZZ,....ZZ$..ZZ$...ZZZ..$ZZ..$ZZ...?ZZZZ..ZZZ....ZZ7.ZZZ.....=ZZZ$..ZZ........",
    "...ZZZZ$ZZZZ...ZZ$...ZZZ..$ZZ..$ZZ....$ZZZ..?ZZZ+$ZZZ..ZZZ.....=ZZZ$..ZZZZ77$Z..",
    "....:ZZZZZ~....ZZZ...ZZZ..$ZZ..ZZZ.....ZZZ....ZZZZZ:...ZZZ......7ZZZ...:ZZZZZZ..",
    "................................................................................"
};
void dibujarMarco() {
    const int ancho = 100; // Ajusta según el tamaño de tu consola
    const int alto = 28;   // Ajusta según la altura de tu consola
    const int x0 = 0;
    const int y0 = 0;

    setColor(1); // Blanco brillante

    // Bordes horizontales
    for (int x = x0; x < ancho; ++x) {
        gotoxy(x, y0); cout << (char)10000;               // Línea superior ─
        gotoxy(x, y0 + alto); cout << (char)10000;        // Línea inferior ─
    }

    // Bordes verticales
    for (int y = y0; y <= alto; ++y) {
        gotoxy(x0, y); cout << (char)186;               // Línea izquierda │
        gotoxy(x0 + ancho - 1, y); cout << (char)10000;   // Línea derecha  │
    }

    // Esquinas
    gotoxy(x0, y0); cout << (char)201;                      // Esquina superior izquierda ┌
    gotoxy(x0 + ancho - 1, y0); cout << (char)10000;          // Esquina superior derecha ┐
    gotoxy(x0, y0 + alto); cout << (char)10000;               // Esquina inferior izquierda └
    gotoxy(x0 + ancho - 1, y0 + alto); cout << (char)10000;   // Esquina inferior derecha ┘

    setColor(7); // Restaurar color normal
}


// ====== ESTRUCTURAS Y CONSTANTES ======
struct Pregunta {
    string texto;
    string opciones[4];
    char respuestaCorrecta;
    string dificultad;
};
constexpr int MAX_PREGUNTAS = 50;
// ====== FUNCION ANIMACION DEL LOGO ======
void animarLogo() {
    int x = 5; // Posición horizontal
    int y = 5; // Posición vertical
    int lineas = sizeof(logo) / sizeof(logo[0]);
    int colores[] = {11, 11, 11, 11, 11, 11}; // Aqua, Magenta, Amarillo, Verde, Azul, Rojo
    int nColores = sizeof(colores) / sizeof(colores[0]);

    for (int i = 0; i < lineas; ++i) {
        setColor(colores[i % nColores]);  // Color cíclico
        gotoxy(x, y + i);
        cout << logo[i] << endl;
    }
     dibujarMarco(); // Muestra marco antes del menú
    setColor(7); // Reset color default
    Sleep(1500); // Espera 1.5 segundos para que se vea el logo
    system("cls"); // Limpia pantalla para el menú
   
}
// ====== FUNCIONES DEL JUEGO ======
void mostrarPregunta(const Pregunta& pregunta) {
    cout << "\nDificultad: " << pregunta.dificultad << "\n\n";
    cout << pregunta.texto << "\n\n";
    cout << "A) " << pregunta.opciones[0] << "\n";
    cout << "B) " << pregunta.opciones[1] << "\n";
    cout << "C) " << pregunta.opciones[2] << "\n";
    cout << "D) " << pregunta.opciones[3] << "\n\n";
}

bool cargarPreguntasDesdeArchivo(const string& nombreArchivo, Pregunta preguntas[], int& cantidad) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de preguntas: " << nombreArchivo << endl;
        return false;
    }
    cantidad = 0;
    string linea;
    while (getline(archivo, linea) && cantidad < MAX_PREGUNTAS) {
        Pregunta p;
        p.texto = linea;
        for (int i = 0; i < 4; ++i) {
            if (!getline(archivo, p.opciones[i])) return false;
        }
        if (!getline(archivo, linea)) return false;
        p.respuestaCorrecta = toupper(linea[0]);
        if (!getline(archivo, p.dificultad)) return false;
        preguntas[cantidad++] = p;
    }
    archivo.close();
    return true;
}

vector<Pregunta> filtrarPorDificultad(Pregunta preguntas[], int cantidad, const string& dificultad) {
    vector<Pregunta> filtradas;
    for (int i = 0; i < cantidad; ++i) {
        if (preguntas[i].dificultad == dificultad) {
            filtradas.push_back(preguntas[i]);
        }
    }
    // Usar std::shuffle con std::random_device y std::mt19937
    random_device rd;
    mt19937 g(rd());
    shuffle(filtradas.begin(), filtradas.end(), g);
    return filtradas;
}

void guardarPuntaje(const string& nombre, int puntaje) {
    ofstream archivo("puntajes.txt", ios::app);
    if (archivo) archivo << nombre << ": " << puntaje << " puntos\n";
    else cerr << "Error al guardar el puntaje.\n";
}

void borrarPuntajes() {
    ofstream archivo("puntajes.txt", ios::trunc);
    if (archivo) cout << "\nPuntajes borrados exitosamente.\n";
    else cerr << "\nNo se pudo borrar el archivo de puntajes.\n";
}
// ====== MENSAJE FINAL SEGUN PUNTAJE ======
void mostrarMensajeFinal(int puntaje) {
    cout << "\n=== Mensaje Final ===\n";
    if (puntaje <= 3) {
        setColor(12); // rojo
        cout << "No te rindas, sigue practicando.\n";
    } else if (puntaje <= 7) {
        setColor(14); // amarillo
        cout << "¡Bien hecho! Buen trabajo.\n";
    } else {
        setColor(10); // verde
        cout << "¡Excelente! Eres un experto.\n";
    }
    setColor(7);
}
// ====== FUNCION DE JUEGO CON CONTADOR Y LECTURA NO BLOQUEANTE ======
void jugarTrivia(const string& jugador, const vector<Pregunta>& preguntas, int& puntaje) {
    for (size_t i = 0; i < preguntas.size(); ++i) {
        system("cls");
        cout << "\nTurno de " << jugador << "\n";
        mostrarPregunta(preguntas[i]);

        char respuesta = '\0';
        int tiempoRestante = 10;
        bool respondio = false;

        cout << "Tienes 10 segundos para responder. Tiempo restante: " << tiempoRestante << " seg." << endl;
        cout << "Tu respuesta (A, B, C o D): ";

        // Bucle de 10 segundos con contador visible
        for (int segundos = 10; segundos > 0; --segundos) {
            Sleep(1000);
            tiempoRestante--;

            // Mostrar tiempo restante en la misma línea (usar \r para regresar el cursor)
            cout << "\rTienes " << tiempoRestante << " segundos para responder. Presiona A, B, C o D: ";
            cout.flush();

            if (_kbhit()) { // Si presionó una tecla
                char c = _getch(); // obtener la tecla sin mostrarla
                c = toupper(c);
                if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
                    respuesta = c;
                    respondio = true;
                    break;
                }
            }
        }

        if (!respondio) {
            // Se acabó el tiempo sin responder
            setColor(12);
            cout << "\n\n¡Tiempo agotado! La respuesta correcta era: " << preguntas[i].respuestaCorrecta << "\n";
            PlaySound(TEXT("incorrecta.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido incorrecto
            setColor(7);
        } else {
            // Procesar la respuesta
            if (respuesta == preguntas[i].respuestaCorrecta) {
                setColor(10);
                cout << "\n\n¡Correcto!\n";
                PlaySound(TEXT("correcto.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido correcto
                ++puntaje;
            } else {
                setColor(12);
                cout << "\n\nIncorrecto. La respuesta correcta era: " << preguntas[i].respuestaCorrecta << "\n";
                PlaySound(TEXT("incorrecta.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido incorrecto
            }
            setColor(7);
        }

        cout << "\nPresiona ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar buffer
        cin.get();
    }
}
// ====== FUNCION PARA SELECCIONAR MATERIA ======
void seleccionarMateria(string& archivoMateria) {
    int op;
    cout << "\nSeleccione la materia:\n";
    cout << "1. Programacion\n2. Fisica 1\n3. Geometria Analitica\nOpcion: ";
    cin >> op;
    switch (op) {
        case 1: archivoMateria = "preguntas_programacion.txt"; break;
        case 2: archivoMateria = "preguntas_fisica1.txt"; break;
        case 3: archivoMateria = "preguntas_geometria.txt"; break;
        default: archivoMateria = "preguntas_programacion.txt"; break;
    }
}
// ====== MODO UN JUGADOR ======
void jugarTriviaUnJugador() {
    system("cls");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar buffer
    string nombre;
    cout << "\nNombre del jugador: ";
    getline(cin, nombre);
    string dificultad;
    cout << "Selecciona la dificultad (facil, medio, dificil): ";
    cin >> dificultad;

    string archivoMateria;
    seleccionarMateria(archivoMateria);

    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas;
    if (!cargarPreguntasDesdeArchivo(archivoMateria, preguntas, cantidadPreguntas)) {
        cout << "Error cargando preguntas.\n";
        system("pause");
        return;
    }

    vector<Pregunta> filtradas = filtrarPorDificultad(preguntas, cantidadPreguntas, dificultad);
    if (filtradas.empty()) {
        cout << "No hay preguntas para esa dificultad.\n";
        system("pause");
        return;
    }

    int puntaje = 0;
    jugarTrivia(nombre, filtradas, puntaje);
   cout << "\n";
setColor(13); // Magenta claro
gotoxy(30, 20); cout << "=== RESULTADO FINAL ===";
setColor(14); // Amarillo claro
gotoxy(30, 22); cout << nombre << ": ";
setColor(10); // Verde brillante
cout << puntaje << " puntos\n";
setColor(7); // Restaurar color

    mostrarMensajeFinal(puntaje);
    guardarPuntaje(nombre, puntaje);
}
// ====== MODO DOS JUGADORES ======
void jugarTriviaDosJugadores() {
    system("cls");
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar buffer
    string nombre1, nombre2;
    cout << "\nNombre del Jugador 1: ";
    getline(cin, nombre1);
    cout << "Nombre del Jugador 2: ";
    getline(cin, nombre2);

    string dificultad;
    cout << "Selecciona la dificultad (facil, medio, dificil): ";
    cin >> dificultad;

    string archivoMateria;
    seleccionarMateria(archivoMateria);

    Pregunta preguntas[MAX_PREGUNTAS];
    int cantidadPreguntas;
    if (!cargarPreguntasDesdeArchivo(archivoMateria, preguntas, cantidadPreguntas)) {
        cout << "Error cargando preguntas.\n";
        system("pause");
        return;
    }

    vector<Pregunta> filtradas = filtrarPorDificultad(preguntas, cantidadPreguntas, dificultad);
    if (filtradas.empty()) {
        cout << "No hay preguntas para esa dificultad.\n";
        system("pause");
        return;
    }

    int puntaje1 = 0, puntaje2 = 0;
    for (size_t i = 0; i < filtradas.size(); ++i) {
        system("cls");
        string jugadorActual = (i % 2 == 0) ? nombre1 : nombre2;
        int& puntajeActual = (i % 2 == 0) ? puntaje1 : puntaje2;
        cout << "\nTurno de " << jugadorActual << "\n";
        mostrarPregunta(filtradas[i]);

        char respuesta = '\0';
        int tiempoRestante = 10;
        bool respondio = false;

        cout << "Tienes 10 segundos para responder. Tiempo restante: " << tiempoRestante << " seg." << endl;
        cout << "Tu respuesta (A, B, C o D): ";

        for (int segundos = 10; segundos > 0; --segundos) {
            Sleep(1000);
            tiempoRestante--;
            cout << "\rTienes " << tiempoRestante << " segundos para responder. Presiona A, B, C o D: ";
            cout.flush();

            if (_kbhit()) {
                char c = _getch();
                c = toupper(c);
                if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
                    respuesta = c;
                    respondio = true;
                    break;
                }
            }
        }

        if (!respondio) {
            setColor(12);
            cout << "\n\n¡Tiempo agotado! La respuesta correcta era: " << filtradas[i].respuestaCorrecta << "\n";
            PlaySound(TEXT("incorrecta.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido incorrecto
            setColor(7);
        } else {
            if (respuesta == filtradas[i].respuestaCorrecta) {
                setColor(10);
                cout << "\n\n¡Correcto!\n";
                PlaySound(TEXT("correcto.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido correcto
                ++puntajeActual;
            } else {
                setColor(12);
                cout << "\n\nIncorrecto. La respuesta correcta era: " << filtradas[i].respuestaCorrecta << "\n";
                PlaySound(TEXT("incorrecta.wav"), NULL, SND_FILENAME | SND_ASYNC); // Reproducir sonido incorrecto
            }
            setColor(7);
        }

        cout << "\nPresiona ENTER para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    setColor(13); // Magenta claro
gotoxy(37, 20); cout << "=== RESULTADOS FINALES ===";

setColor(14);
gotoxy(30, 22); cout << nombre1 << ": ";
setColor(10); cout << puntaje1 << " puntos";
mostrarMensajeFinal(puntaje1);

setColor(14);
gotoxy(30, 24); cout << nombre2 << ": ";
setColor(10); cout << puntaje2 << " puntos";
mostrarMensajeFinal(puntaje2);

setColor(7);

    guardarPuntaje(nombre1, puntaje1);
    guardarPuntaje(nombre2, puntaje2);
}
// ====== INTERFAZ Y MENU ======
void mostrarBanner() {
    setColor(13);
    gotoxy(30, 2); cout << "=========================";
    gotoxy(30, 3); cout << "           TRIVIA          ";
    gotoxy(30, 4); cout << "=========================";
    setColor(7);
}

void mostrarMenu() {
    int opcion;
    do {
        system("cls");
        dibujarMarco(); 
        mostrarBanner();
        int x = 30, y = 6;
        gotoxy(x, y++); setColor(11); cout << "=========== MENU PRINCIPAL ===========";
        gotoxy(x, y++); setColor(14); cout << " 1. Ver Instrucciones";
        gotoxy(x, y++); cout << " 2. Jugar (Un jugador)";
        gotoxy(x, y++); cout << " 3. Jugar (Dos jugadores)";
        gotoxy(x, y++); cout << " 4. Ver Puntajes";
        gotoxy(x, y++); cout << " 5. Borrar Puntajes";
        gotoxy(x, y++); setColor(7); cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear(); cin.ignore(10000, '\n');
            setColor(12);
            cout << "\nEntrada invalida.\n";
            setColor(7);
            Sleep(1000);
            continue;
        }

        switch (opcion) {
            case 1:
                system("cls");
                cout << "Instrucciones:\n";
                cout << "- Selecciona la materia y dificultad.\n";
                cout << "- Tienes 10 segundos por pregunta.\n";
                cout << "- Cada respuesta correcta suma 1 punto.\n";
                system("pause");
                break;
            case 2:
                jugarTriviaUnJugador();
                system("pause");
                break;
            case 3:
                jugarTriviaDosJugadores();
                system("pause");
                break;
            case 4: {
    system("cls");
    dibujarMarco();

    ifstream archivo("puntajes.txt");
    if (!archivo.is_open()) {
        setColor(12);
        gotoxy(35, 12); cout << "No se pudo abrir el archivo de puntajes.";
        setColor(7);
        system("pause");
        break;
    }

    vector<string> lineas;
    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty())
            lineas.push_back(linea);
    }
    archivo.close();

    setColor(13); // Magenta claro
    gotoxy(35, 4); cout << "=== PUNTAJES ===";
    setColor(7);

    if (lineas.empty()) {
        setColor(12); // Rojo
        gotoxy(32, 10); cout << "No hay puntajes registrados aun.";
        setColor(7);
    } else {
        int y = 7;
        for (const string& texto : lineas) {
            gotoxy(30, y++); // Centrado horizontal aproximado
            setColor(14);    // Amarillo
            cout << texto;
        }
    }

    setColor(7);
    gotoxy(30, 25); cout << "Presiona cualquier tecla para volver al menu...";
    _getch();
    break;
}

            case 5:
                borrarPuntajes();
                system("pause");
                break;
            case 6:
                cout << "\nGracias por jugar.\n";
                break;
            default:
                setColor(12); cout << "\nOpcion no valida.\n";
                setColor(7); Sleep(1000);
        }
    } while (opcion != 6);
}
// ====== MAIN ======
int main() { // main sin argumentos para SDL
    srand(static_cast<unsigned int>(time(NULL)));
    animarLogo(); // Muestra logo animado antes del menu
    mostrarMenu();
    return 0;
}

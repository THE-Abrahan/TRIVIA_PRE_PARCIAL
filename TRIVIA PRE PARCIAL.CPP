#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <windows.h>
#include <algorithm>
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
    "............................:++~,.............+????+............................",
    "........................=+=.....................~????,..........................",
    ".....................+?=.........................+??++..........................",
    "..................~++......$=....................+???,..........................",
    ".ZZZZ.....ZZZ...+?=.......ZZZ..$ZZZ.....ZZZ$....~???~............?..............",
    "..ZZZ.....ZZZ..................$ZZZZ....ZZ.....................ZZZ..............",
    "..ZZZ.....ZZZ..ZZ.OZZZ7..ZZZZ..$ZZZZZ...ZZ....ZZZZZ7...$Z+?Z$.ZZZZZZ...~ZZZZZ...",
    "..ZZZ.....ZZZ..$ZZZ=ZZZZ.IZZZ..$ZZ.ZZZ..ZZ..7ZZZ.:ZZZ..ZZZZO$.7ZZZ$$..ZZZ..ZZZ..",
    "..ZZZ.....ZZ$..ZZZ...ZZZ..$ZZ..$ZZ..ZZZ.ZZ..ZZZ....ZZ$.ZZZ.....ZZZ...:ZZ....ZZ=.",
    "..ZZZ.....ZZZ..ZZ$...ZZZ..$ZZ..$ZZ..:ZZ=ZZ..ZZ=....ZZZ.ZZZ.....ZZZ...ZZZZZZZZZI.",
    "..+ZZ,....ZZ$..ZZ$...ZZZ..$ZZ..$ZZ...?ZZZZ..ZZZ....ZZ7.ZZZ.....ZZZ...=ZZ........",
    "...ZZZZ$ZZZZ...ZZ$...ZZZ..$ZZ..$ZZ....$ZZZ..?ZZZ+$ZZZ..ZZZ.....=ZZZ$..ZZZZ77$Z..",
    "....:ZZZZZ~....ZZZ...ZZZ..$ZZ..ZZZ.....ZZZ....ZZZZZ:...ZZZ......7ZZZ...:ZZZZZZ..",
    "................................................................................"
};

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
    int colores[] = {1, 1, 1, 1, 1, 1}; // Aqua, Magenta, Amarillo, Verde, Azul, Rojo
    int nColores = sizeof(colores) / sizeof(colores[0]);

    for (int i = 0; i < lineas; ++i) {
        setColor(colores[i % nColores]);  // Color cíclico
        gotoxy(x, y + i);
        cout << logo[i] << endl;
    }
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

bool procesarRespuesta(char respuestaUsuario, const Pregunta& pregunta, double tiempo, int& puntaje) {
    respuestaUsuario = toupper(respuestaUsuario);
    if (tiempo > 10.0) {
        setColor(12);
        cout << "\n¡Tiempo agotado! La respuesta correcta era: " << pregunta.respuestaCorrecta << "\n";
        return false;
    }
    else if (respuestaUsuario == pregunta.respuestaCorrecta) {
        setColor(10);
        cout << "\n¡Correcto!\n";
        ++puntaje;
        return true;
    }
    else {
        setColor(12);
        cout << "\nIncorrecto. La respuesta correcta era: " << pregunta.respuestaCorrecta << "\n";
        return false;
    }
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
    random_shuffle(filtradas.begin(), filtradas.end());
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

void jugarTrivia(const string& jugador, const vector<Pregunta>& preguntas, int& puntaje) {
    for (size_t i = 0; i < preguntas.size(); ++i) {
        system("cls");
        cout << "\nTurno de " << jugador << "\n";
        mostrarPregunta(preguntas[i]);
        char respuesta;
        clock_t inicio = clock();
        cout << "Tu respuesta (10 segundos): ";
        cin >> respuesta;
        clock_t fin = clock();
        double tiempo = double(fin - inicio) / CLOCKS_PER_SEC;
        procesarRespuesta(respuesta, preguntas[i], tiempo, puntaje);
        setColor(7);
        cout << "\nPresiona ENTER para continuar...";
        cin.ignore();
        cin.get();
    }
}

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
    cin.ignore();
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
    cout << "\n=== Resultado ===\n" << nombre << ": " << puntaje << " puntos\n";
    guardarPuntaje(nombre, puntaje);
}

// ====== MODO DOS JUGADORES ======
void jugarTriviaDosJugadores() {
    system("cls");
    cin.ignore();
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
        char respuesta;
        clock_t inicio = clock();
        cout << "Tu respuesta (10 segundos): ";
        cin >> respuesta;
        clock_t fin = clock();
        double tiempo = double(fin - inicio) / CLOCKS_PER_SEC;
        procesarRespuesta(respuesta, filtradas[i], tiempo, puntajeActual);
        setColor(7);
        cout << "\nPresiona ENTER para continuar...";
        cin.ignore();
        cin.get();
    }

    cout << "\n=== Resultados Finales ===\n";
    cout << nombre1 << ": " << puntaje1 << " puntos\n";
    cout << nombre2 << ": " << puntaje2 << " puntos\n";
    guardarPuntaje(nombre1, puntaje1);
    guardarPuntaje(nombre2, puntaje2);
}

// ====== INTERFAZ Y MENU ======
void mostrarBanner() {
    setColor(13);
    gotoxy(30, 2); cout << "=========================";
    gotoxy(30, 3); cout << "      TRIVIA  Pre_Parcial        ";
    gotoxy(30, 4); cout << "=========================";
    setColor(7);
}

void mostrarMenu() {
    int opcion;
    do {
        system("cls");
        mostrarBanner();
        int x = 30, y = 6;
        gotoxy(x, y++); setColor(11); cout << "=========== MENU PRINCIPAL ===========";
        gotoxy(x, y++); setColor(14); cout << " 1. Ver Instrucciones";
        gotoxy(x, y++); cout << " 2. Jugar (Un jugador)";
        gotoxy(x, y++); cout << " 3. Jugar (Dos jugadores)";
        gotoxy(x, y++); cout << " 4. Ver Puntajes";
        gotoxy(x, y++); cout << " 5. Borrar Puntajes";
        gotoxy(x, y++); cout << " 6. Salir";
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
                ifstream archivo("puntajes.txt");
                string linea;
                cout << "\n=== Puntajes ===\n";
                while (getline(archivo, linea)) cout << linea << endl;
                system("pause");
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
int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    animarLogo(); // Muestra logo animado antes del menu
    mostrarMenu();
    return 0;
}

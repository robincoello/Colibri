/*
The MIT License (MIT)

Copyright (c) 2015 - Latino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef _AST_H_
#define _AST_H_

#include <stdbool.h>
#include "object.h"
#include "vm.h"

/** \file ast.h
*
* Contiene las estructuras y metodos necesarios para el manejo del analizador lexico sintactico
*
*/

/**\brief Bandera para debuguear las instrucciones que se generan con el AST */
#define DEPURAR_AST 0

/** \brief Tipos de nodos en arbol abstracto de sintaxis (abstract syntax tree) */
typedef enum
{
    NODO_NULO,      /**< No se usa pero se reserva para agregar nulo */
    NODO_VALOR,     /**< El nodo contiene valores (logico | numerico | cadena) */
    NODO_MAS_UNARIO,  /**< Nodo mas unario */
    NODO_MENOS_UNARIO,  /**< Nodo menos unario */
    NODO_INC,       /**< Nodo incremento i++*/
    NODO_DEC,       /**< Nodo decremento i--*/
    NODO_SUMA,    /**< Nodo suma */
    NODO_RESTA,    /**< Nodo resta */
    NODO_MULTIPLICACION,    /**< Nodo multiplicacion */
    NODO_DIVISION,    /**< Nodo division */
    NODO_MODULO,  /**< Nodo modulo */
    NODO_MAYOR_QUE,  /**< Nodo mayor que */
    NODO_MAYOR_IGUAL,  /**< Nodo mayor o igual */
    NODO_MENOR_QUE,  /**< Nodo menor que */
    NODO_MENOR_IGUAL,  /**< Nodo menor o igual */
    NODO_IGUALDAD,  /**< Nodo igualdad */
    NODO_DESIGUALDAD,  /**< Nodo desigualdad */
    NODO_Y,   /**< Nodo y (and) logico */
    NODO_O,   /**< Nodo o (or) logico */
    NODO_NO,    /**< Nodo no (!) negacion logico */
    NODO_IDENTIFICADOR,  /**< Nodo identificador */
    NODO_BLOQUE,  /**< Nodo bloque */
    NODO_ASIGNACION,  /**< Nodo asignacion */
    NODO_SI,  /**< Nodo si */
    NODO_ELEGIR,   /**< Nodo seleccion */
    NODO_CASO,   /**< Nodo caso */
    NODO_DEFECTO,   /**< Nodo defecto */
    NODO_CASOS,   /**< Nodo casos */
    NODO_MIENTRAS,  /**< Nodo mientras */
    NODO_HACER,  /**< Nodo hacer */
    NODO_DESDE,   /**< Nodo desde */
    NODO_FUNCION_PARAMETROS,  /**< Nodo parametros */
    NODO_FUNCION_ARGUMENTOS,  /**< Nodo argumentos de una funcion */
    NODO_FUNCION_LLAMADA,  /**< Nodo llamada a una funcion */
    NODO_FUNCION_USUARIO,  /**< Nodo definicion de una funcion de usuario*/
    NODO_RETORNO,  /**< Nodo retorno */
    NODO_CONCATENAR,   /**< Nodo concatenar */
    NODO_LISTA,   /**< Nodo lista nueva */
    NODO_LISTA_ASIGNAR_ELEMENTO,   /**< Nodo asignar un nuevo valor a un elemento de la lista */
    NODO_LISTA_AGREGAR_ELEMENTO,   /**< Nodo agregar elemento a una lista */
    NODO_LISTA_OBTENER_ELEMENTO,   /**< Nodo obtener un elemento de una lista */
    NODO_DICCIONARIO,   /**< Nodo diccionario nuevo */
    //NODO_DICC_ASIGNAR_ELEMENTO,   /**< Nodo un nuevo valor a un elemento del diccionario */
    NODO_DICC_AGREGAR_ELEMENTO,   /**< Nodo agregar un elemento al diccionario */
    //NODO_DICC_OBTENER_ELEMENTO,   /**< Nodo obtener un elemento del diccionario */
    NODO_DICC_ELEMENTO,   /**< Nodo par de llave valor del diccionario (key : value) */
    NODO_INCLUIR,  /**< Nodo para incluir codigo de otro archivo */
    NODO_ATRIBUTO,  /**< Attributo de un objeto: str->longitud() */
} nodo_tipo;

/** \brief Tipos de dato */
typedef enum
{
    VALOR_NULO,     /**< Valor nulo */
    VALOR_LOGICO,   /**< Valor logico */
    VALOR_NUMERICO, /**< Valor decimal */
    VALOR_CADENA    /**< Valor cadena */
} nodo_tipo_valor;

#define AST_COMUN \
        nodo_tipo tipo; \
        int num_linea; \
        int num_columna;

/** \brief Valores del dato */
typedef struct
{
    nodo_tipo_valor tipo; /**< Nodo tipo valor */
    bool es_constante;  /**< para validar si un identificador es constante */
    /** Contiene los valores del dato*/
    union val
    {
        bool logico;  /**< Logico */
        double numerico;   /**< Decimal */
        char *cadena;    /**< Cadena */
    } val;
} nodo_valor;

/** \brief Nodos en arbol abstacto de sintaxis (abstract syntax tree).
  *
  * Todos los nodos son inicializados con un tipo de nodo */
typedef struct ast
{
    AST_COMUN
    nodo_valor *valor; /**< Valor del nodo */
    struct ast *izq; /**< Nodo izquierdo */
    struct ast *der; /**< Nodo derecho */
    lat_mv *vm;
} ast;

/** \brief Estado del analizador lexico */
typedef struct lex_state
{
    int insert;
} lex_state;

/** \brief Tipo de dato que se envia al analizador lexico */
typedef union YYSTYPE
{
    int token;
    ast *node;
} YYSTYPE;

/** \brief nodo para representar un ast SI (if).
  *
  * si (condicion) [sentencias] sino [sentencias] fin */
typedef struct
{
    AST_COMUN
    struct ast *condicion; /**< Condicion */
    struct ast *entonces;   /**< Instrucciones que se ejecutan si la condicion es verdadera */
    struct ast *_sino;   /**< Instrucciones que se ejecutan si la condicion es falsa */
} nodo_si;

/** \brief nodo para representar una funcion.
  *
  * funcion nombre (parametros) [sentencias] fin */
typedef struct
{
    AST_COMUN
    struct ast *nombre; /**< Nombre de la funcion */
    struct ast *parametros;   /**< Parametros de la funcion */
    struct ast *sentencias;   /**< Instrucciones que se ejecutan si se llama a la funcion */
} nodo_funcion;

/** \brief Nodo para el elemento de una lista */
typedef struct
{
    AST_COMUN
    struct ast *expresion;  /**< Expresion */
    struct ast *identificador;   /**< Identificador */
    struct ast *posicion;  /**< Posicion de la lista */
} nodo_lista_elem;

/** \brief Nodo para el elemento de un diccionario */
typedef struct
{
    AST_COMUN
    struct ast *expresion;  /**< Expresion */
    struct ast *identificador;   /**< Identificador */
    struct ast *llave;  /**< Llave del diccionario */
} nodo_dicc_elem;


/** \brief Nuevo nodo generico para el AST
  *
  * \param tipo: Tipo de nodo
  * \param l: Nodo izquierdo
  * \param r: Nodo derecho
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo(nodo_tipo tipo, ast *l, ast *r);

/** Nuevo nodo tipo Identificador (var)
  *
  * \param s: nombre del identificador
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_identificador(const char *s, int num_linea, int num_columna, bool es_constante);

/** Nuevo nodo tipo Logico (verdadero/falso)
  *
  * \param b: Nodo valor (0 o 1)
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_logico(int b, int num_linea, int num_columna);

/** Nuevo nodo tipo nulo
  *
  * \param b: Nodo valor (NULL)
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_nulo(void* nulo, int num_linea, int num_columna);

/** Nuevo nodo tipo Decimal (1.25)
  *
  * \param d: Nodo valor
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_numerico(double d, int num_linea, int num_columna);

/** Nuevo nodo tipo Cadena ("Esto es una 'cadena'")
  *
  * \param s: Nodo valor
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_cadena(const char *s, int num_linea, int num_columna);

/** Nuevo nodo tipo Operador (var1 + var2)
  *
  * \param nt: tipo de nodo
  * \param l: Nodo izquierdo
  * \param r: Nodo derecho
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_operador(nodo_tipo nt, ast *l, ast *r);

/** Nuevo nodo tipo Asignacion (var = "hola latino")
  *
  * \param s: Nodo identificador
  * \param v: Nodo valor
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_asignacion(ast *val, ast *sim);

/** Nuevo nodo tipo Asignacion Lista (lista[pos] = exp)
  *
  * \param s: Nodo identificador de la lista
  * \param v: Nodo valor o expresion
  * \param pos: Nodo posicion en la lista
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_asignacion_lista_elem(ast *s, ast *v, ast *pos);

/** Nuevo nodo tipo Asignacion Diccionario (dicc[llave] = exp)
  *
  * \param s: Nodo identificador del diccionario
  * \param v: Nodo valor o expresion
  * \param llave: Nodo llave del diccionario
  * \return ast: Un nodo AST
  *
  */
//ast *nodo_nuevo_asignacion_dicc_elem(ast *s, ast *v, ast *llave);

/** Nuevo nodo tipo si (if)
  *
  * \param cond: Nodo condicion ( a > b && a > c )
  * \param th: Nodo lista de sentencias (en caso de ser verdadera la condicion)
  * \param el: Nodo lista de sentencias (en caso de ser falsa la condicion)
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_si(ast *cond, ast *th, ast *el);

/** Nuevo nodo tipo mientras (while)
  *
  * \param cond: Nodo condicion ( a > b && a > c )
  * \param stmts: Nodo lista de sentencias (mientras sea verdadera la condicion)
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_mientras(ast *cond, ast *stmts);

/** Nuevo nodo tipo hacer (do)
  *
  * \param cond: Nodo condicion ( a > b && a > c )
  * \param stmts: Nodo lista de sentencias (mientras sea verdadera la condicion)
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_hacer(ast *cond, ast *stmts);

/** Nuevo nodo tipo desde (for)
  *
  * \param dec: Nodo declaracion ( i = 0 )
  * \param cond: Nodo condicion ( i <= 10 )
  * \param inc: Nodo incremento ( i++ )
  * \param stmts: Nodo lista de sentencias mientras se cumpla la condicion
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_desde(ast *dec, ast *cond, ast *inc, ast *stmts);

/** Nuevo nodo tipo funcion
  *
  * \param nombre: Nodo nombre de la funcion
  * \param parametros: Nodo parametros de la funcion
  * \param sentencias: Nodo lista de sentencias de la funcion
  * \return ast: Un nodo AST
  *
  */
ast *nodo_nuevo_funcion(ast *nombre, ast *parametros, ast *sentencias);

/** Nuevo nodo que incluye un modulo
  *
  * \param name: Nodo nombre del modulo
  * \return ast: Un nodo AST
  *
  */
ast* nodo_nuevo_incluir(ast* ruta);

/** Reduce constantes por ej. (1 + 2) lo reduce a un solo nodo (3)
  *
  * \param nt: Tipo del nodo
  * \param lhs: Nodo izquierdo
  * \param lhs: Nodo derecho
  * \return ast: Un nodo AST
  *
  */
ast* nodo_reducir_constantes(nodo_tipo nt, ast* lhs, ast* rhs);

/** Libera la memoria creada dinamicamente
  *
  * \param a: Nodo AST
  *
  */
void nodo_liberar(ast *a);

/** Analiza el arbol abstracto de sintaxis
  *
  * \param vm: Referencia a un objeto tipo máquina virtual
  * \param tree: Arbol abstracto de sintaxis
  * \return lat_object: objeto generico
  *
  */
lat_objeto *nodo_analizar_arbol(lat_mv *vm, ast *tree);

#endif /*_AST_H_*/

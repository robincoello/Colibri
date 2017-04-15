#include "liblist.h"
#include "latino.h"
#include "libmem.h"

lista *__lista_crear()
{ 
    //printf("__lista_crear\n");
    return __memoria_asignar(sizeof(lista));
}

void __lista_destruir(lista *list)
{
    LIST_FOREACH(list, primero, siguiente, cur) {
        if(cur->anterior) {
            __memoria_liberar(cur->anterior);
        }
    }
    __memoria_liberar(list->ultimo);
    __memoria_liberar(list);
}

void __lista_limpiar(lista *list)
{
    LIST_FOREACH(list, primero, siguiente, cur) {
        __memoria_liberar(cur->valor);
    }
}

void __lista_limpiar_destruir(lista *list)
{
    __lista_limpiar(list);
    __lista_destruir(list);
}

void __lista_apilar(lista *list, void *value)
{    
    lista_nodo *node = __memoria_asignar(sizeof(lista_nodo));
    node->valor = value;
    if(list->ultimo == NULL) {
        list->primero = node;
        list->ultimo = node;
    } else {
        list->ultimo->siguiente = node;
        node->anterior = list->ultimo;
        list->ultimo = node;
    }
    list->longitud++;
}

void *__lista_desapilar(lista *list)
{
    lista_nodo *node = list->ultimo;
    return node != NULL ? __lista_eliminar_elemento(list, node) : NULL;
}

void __lista_insertar_inicio(lista *list, void *value)
{
    lista_nodo *node = __memoria_asignar(sizeof(lista_nodo));
    node->valor = value;
    if(list->primero == NULL) {
        list->primero = node;
        list->ultimo = node;
    } else {
        node->siguiente = list->primero;
        list->primero->anterior = node;
        list->primero = node;
    }
    list->longitud++;
}

void *__lista_extraer_inicio(lista *list)
{
    lista_nodo *node = list->primero;
    return node != NULL ? __lista_eliminar_elemento(list, node) : NULL;
}

void *__lista_eliminar_elemento(lista *list, lista_nodo *node)
{
    void *result = NULL;    
    if(node == list->primero && node == list->ultimo) {
        list->primero = NULL;
        list->ultimo = NULL;
    } else if(node == list->primero) {
        list->primero = node->siguiente;
        list->primero->anterior = NULL;
    } else if (node == list->ultimo) {
        list->ultimo = node->anterior;
        list->ultimo->siguiente = NULL;
    } else {
        lista_nodo *after = node->siguiente;
        lista_nodo *before = node->anterior;
        after->anterior = before;
        before->siguiente = after;
    }
    list->longitud--;
    result = node->valor;
    __memoria_liberar(node);
    return result;
}

char* __lista_a_cadena(lista* list)
{
    char* valor = __memoria_asignar(MAX_STR_LENGTH);    
    strcat(valor, "[");
    LIST_FOREACH(list, primero, siguiente, cur) {
        lat_objeto* o = ((lat_objeto*)cur->valor);
        char* str = __objeto_a_cadena(o);
        if(o->tipo == T_STR){
            if(strstr(__cadena(o), "\"") != NULL){
                strcat(valor, "'");
            }else{
                strcat(valor, "\"");
            }
        }
        strcat(valor, str);
        if(o->tipo == T_STR){
            if(strstr(__cadena(o), "\"") != NULL){
                strcat(valor, "'");
            }else{
                strcat(valor, "\"");
            }
        }
        if(cur != list->ultimo){
            strcat(valor, ", ");
        }
    }
    strcat(valor, "]");
	valor[strlen(valor)] = '\0';
    __memoria_reasignar(valor, strlen(valor)+1);
    return valor;
}

void __lista_modificar_elemento(lista* list, void* data, int pos)
{
    int i = 0;
    if (pos < 0 || pos >= __lista_longitud(list))
    {
        lat_fatal_error("Indice fuera de rango");
    }
    LIST_FOREACH(list, primero, siguiente, cur) {
        if (i == pos)
        {
            cur->valor = data;
        }
        i++;
    }
}

int __lista_comparar(lista* lhs, lista*rhs){
    int res = 0;
    int len1 = __lista_longitud(lhs);
    int len2 = __lista_longitud(rhs);
    if(len1 < len2){
        return -1;
    }
    if(len1 > len2){
        return 1;
    }
    int i;    
    for(i=0; i < len1; i++){
        lat_objeto* tmp1 =  __lista_obtener_elemento(lhs, i);
        lat_objeto* tmp2 =  __lista_obtener_elemento(rhs, i);
        res = __objeto_comparar(tmp1, tmp2);
        if(res < 0){
            return -1;
        }
        if(res > 0){
            return 1;
        }
    }
    return res;
}

void __lista_extender(lista* list1, lista* list2){
    LIST_FOREACH(list2, primero, siguiente, cur) {
        __lista_apilar(list1, cur->valor);
    } 
}
#include <cmath>

#include "Metodos.h"

pair<double, int> Biseccion(double(*f)(double x, const vector<double>& muestra, size_t t),
                            double a0, double b0, CriterioParada cp, double err, unsigned int n,
                            const vector<double>& muestra, size_t t) {
    TFloat a(a0, t), b(b0, t), p(t), p0(t);
    unsigned int i;

    // Aplico el método de bisección hasta n iteraciones
    for(i = 1; i <= n; i++) {
        // Busco punto medio del intervalo
        p = (a + b) / 2.0;

        // Criterio de parada: valor indefinido
        if(isnan(p.dbl())) break;

        // Criterio de parada: error absoluto
        if(cp == ERROR_ABSOLUTO) {
            // Si la longitud del intervalo es menor que la cota superior
            // de error absoluto, tomamos como aproximación hallada
            // el punto medio del intervalo
            if(((b - a) / 2.0).dbl() < err) break;
        }
        
        // Criterio de parada: error relativo
        else {
            // Sólo usamos este criterio a partir de la segunda iteración,
            // de lo contrario no disponemos de aproximación anterior (p0)
            if(i >= 2) {
                if(abs(((p - p0) / p).dbl()) < err) break;
            }
        }

        // Elijo un nuevo intervalo
        if((f(a.dbl(), muestra, t) * f(p.dbl(), muestra, t)) > 0) {
            a = p;
        } else {
            b = p;
        }

        // Guardamos aproximación anterior
        p0 = p;
    }

    // Devolvemos la aproximación hallada y la cantidad de iteraciones realizadas.
    return make_pair(p.dbl(), i > n ? n : i);
}

pair<double, int> Newton(double (*f )(double x, const vector<double>& muestra, size_t t),
                         double (*df)(double x, const vector<double>& muestra, size_t t),
                         double p0, CriterioParada cp, double err, unsigned int n,
                         const vector<double>& muestra, size_t t) {
    TFloat p(t);
    unsigned int i;

    // Aplico el método de Newton hasta n iteraciones
    for(i = 1; i <= n; i++) {
        // Busco nueva aproximación
        p = TFloat(p0, t) - TFloat(f(p0, muestra, t), t) / df(p0, muestra, t);

        // Criterio de parada: valor indefinido
        if(isnan(p.dbl())) break;

        // Criterio de parada: error absoluto
        if(cp == ERROR_ABSOLUTO) {
            // Si el error absoluto aproximado es menor que la cota
            // provista, nos detenemos en la aproximación actual
            if(abs((p - p0).dbl()) < err) break;
        }

        // Criterio de parada: error relativo
        else {
            if(abs(((p - p0) / p).dbl()) < err) break;
        }

        // Actualizo la aproximación inicial
        p0 = p.dbl();
    }

    // Devolvemos la aproximación hallada y la cantidad de iteraciones realizadas.
    return make_pair(p.dbl(), i > n ? n : i);
}

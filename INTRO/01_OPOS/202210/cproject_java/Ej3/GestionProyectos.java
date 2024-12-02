/**
 *
 * @author Usuario
 */
 
import java.util.ArrayList;

public class GestionProyectos {

    private Proyecto[] proyectos;
    private int numProyectos;
    private final int N = 20;

    public GestionProyectos() {
        proyectos = new Proyecto[20];
        numProyectos = 0;
    }

    public void darAltaProyecto(Proyecto p) {
        if (proyectos.length == numProyectos) {
            duplicarVector();
        } else {
            proyectos[numProyectos] = p;
            numProyectos++;
        }
    }

    private void duplicarVector() {
        Proyecto[] aux = new Proyecto[numProyectos * 2];
        for (int i = 0; i < numProyectos; i++) {
            aux[i] = proyectos[i];
        }
        proyectos = aux;
    }

    public void incrementarFondos(String titulo, double incremento) {
        int pos = -1;
        for (int i = 0; i < numProyectos; i++) {
            if (proyectos[i].getTitulo().equals(titulo) == true) {
                pos = i;
            }
        }
        if (pos != -1) {
            proyectos[pos].setFondos(proyectos[pos].getFondos() + incremento);
        }
    }

    public String listar1(double limite) {
        String listado = "";
        for (int i = 0; i < numProyectos; i++) {
            if (proyectos[i].getFondos() >= limite) {
                listado = listado + proyectos[i].toString() + "\n";
            }
        }
        return listado;
    }

    public String listar2() {
        String listado = "";
        ArrayList<Integer> departamentos = new ArrayList<Integer>();

        for (int i = 0; i < numProyectos; i++) {
            darAltaDep(departamentos, proyectos[i].getUnidadGasto());
        }

        for (int i = 0; i < departamentos.size(); i++) {
            listado = listado + "Departamento " + i + ": " + departamentos.get(i) + "\n";
        }

        return listado;
    }

    private int buscarDepartamento(ArrayList<Integer> departamentos, int unidadGasto) {
        for (int i = 0; i < departamentos.size(); i++) {
            if (departamentos.get(i).intValue() == unidadGasto) {
                return i;
            }
        }
        return -1;
    }

    private void darAltaDep(ArrayList<Integer> departamentos, int unidadGasto) {
        if (buscarDepartamento(departamentos, unidadGasto) == -1) {
            departamentos.add(unidadGasto);
        }
    }
}

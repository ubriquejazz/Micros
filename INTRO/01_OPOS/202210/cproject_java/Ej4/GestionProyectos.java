import java.util.ArrayList;

/**
 *
 * @author Usuario
 */
 
public class GestionProyectos {

    private ArrayList<Proyecto> proyectos;


    public GestionProyectos() {
        proyectos = new ArrayList<Proyecto>();
    }

    public void darAltaProyecto(Proyecto p) {
        proyectos.add(p);
    }

    public void incrementarFondos(String titulo, double incremento) {
        int pos = -1;
        for (int i = 0; i < proyectos.size(); i++) {
            if (proyectos.get(i).getTitulo().equals(titulo) == true) {
                pos = i;
            }
        }
        if (pos != -1) {
            double fondosAntiguos = proyectos.get(pos).getFondos();
            double fondosNuevos = fondosAntiguos+incremento;
            proyectos.get(pos).setFondos(fondosNuevos);
        }
    }

    public String listar1(double limite) {
        String listado = "";
        for (int i = 0; i < proyectos.size(); i++) {
            if (proyectos.get(i).getFondos() >= limite) {
                listado = listado + proyectos.get(i).toString() + "\n";
            }
        }
        return listado;
    }

    public String listar2() {
        String listado = "";
        ArrayList<Integer> departamentos = new ArrayList<Integer>();

        for (int i = 0; i < proyectos.size(); i++) {
            darAltaDep(departamentos, proyectos.get(i).getUnidadGasto());
        }

        for (int i = 0; i < departamentos.size(); i++) {
            listado = listado + "Departamento " + (i+1) + ": " + departamentos.get(i) + "\n";
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
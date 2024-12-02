/**
 *
 * @author Usuario
 */
 
public class Proyecto {

    private int         codigo;
    private static int  numCod=0;
    private String      titulo;
    private String      responsable;
    private int         unidadGasto;
    private double      fondos;
    private boolean     vigente;
    
    public Proyecto(String titulo, String responsable, int unidadGasto, double fondos, boolean vigente){
        Proyecto.numCod++;
        this.codigo=Proyecto.numCod;
        this.titulo=titulo;
        this.responsable=responsable;
        this.unidadGasto=unidadGasto;
        this.fondos=fondos;
        this.vigente=vigente;
    }

    public int getCodigo() {
        return codigo;
    }

    public String getTitulo() {
        return titulo;
    }

    public int getUnidadGasto() {
        return unidadGasto;
    }

    public double getFondos() {
        return fondos;
    }

    public boolean isVigente() {
        return vigente;
    }

    public void setCodigo(int codigo) {
        this.codigo = codigo;
    }

    public void setTitulo(String titulo) {
        this.titulo = titulo;
    }

    public void setUnidadGasto(int unidadGasto) {
        this.unidadGasto = unidadGasto;
    }

    public void setFondos(double fondos) {
        this.fondos = fondos;
    }

    public void setVigente(boolean vigente) {
        this.vigente = vigente;
    }

    public String getResponsable() {
        return responsable;
    }

    public void setResponsable(String responsable) {
        this.responsable = responsable;
    }

    @Override
    public String toString() {
        return "Proyecto{" + "codigo=" + codigo + ", titulo=" + titulo + ", unidadGasto=" + unidadGasto + ", fondos=" + fondos + ", vigente=" + vigente + '}';
    }
    
}

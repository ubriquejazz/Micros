import java.util.*;

/**
 *
 * @author Usuario
 */
 
public class Ejercicio4 {
    public static void main(String[]args){
        GestionProyectos gestionProyectos = new GestionProyectos();
	int opcion;
	String titulo;
	String responsable;
	int unidadGasto;
	double fondos;
	boolean vigencia;
	Proyecto nuevo;
	String listado="";
	do{
		mostrarMenu();
		opcion=leerOpcion();
		switch(opcion){
			case 1: 
				System.out.println("Dame el titulo: ");
				titulo = leerCadena();
				System.out.println("Dame el responsable: ");
				responsable = leerCadena();
				System.out.println("Dime la unidad de gasto: ");
				unidadGasto = leerUnidadGasto();
				System.out.println("Dime los fondos: ");
				fondos = leerFondos();
				System.out.println("Dime la vigencia (0 no vigente - 1 vigente): ");
				vigencia = leerVigencia();
				nuevo = new Proyecto(titulo,responsable,unidadGasto,fondos,vigencia);
                                gestionProyectos.darAltaProyecto(nuevo);
			break;
			case 2:
				System.out.println("Dime el título del proyecto: ");
				titulo = leerCadena();
				System.out.println("Dime la cantidad: ");
				fondos = leerFondos();
				gestionProyectos.incrementarFondos(titulo, fondos);
			break;
			case 3: 
				System.out.println("Dime la cantidad limite: ");
				fondos = leerFondos();
				listado = gestionProyectos.listar1(fondos);
                                System.out.println(listado);
			break;
			case 4: 
                            listado = gestionProyectos.listar2();
                            System.out.println(listado);
			break;
			default: 
                            System.out.println("BYE BYE");
		}
		
	}while(opcion!=5);
    }


    public static void mostrarMenu(){
            System.out.println("\nMenú de opciones:");
            System.out.println("\n\tPulse 1 para dar de alta un nuevo proyecto.");
            System.out.println("\n\tPulse 2 para incrementar los fondos de un proyecto.");
            System.out.println("\n\tPulse 3 para listar los proyectos superiores a una cantidad de fondos.");
            System.out.println("\n\tPulse 4 para listar los departamentos (unidades de gasto).");
            System.out.println("\n\tPulse 5 para salir.");
            System.out.println("\n\tElija opion: ");
    }

    public static String leerCadena(){
        Scanner sc = new Scanner(System.in);
        String cadena;
        cadena = sc.next();
        return cadena;
    }

    public static int leerOpcion(){
        Scanner sc = new Scanner(System.in);
        int opcion;
        opcion = sc.nextInt();
        while(opcion<1 || opcion >5){
            System.out.println("Mal, repite: ");
            opcion = sc.nextInt();
        }
        return opcion;
    }

    public static double leerFondos(){
        Scanner sc = new Scanner(System.in);
        double fondos;
        fondos = sc.nextDouble();
        while(fondos<0){
            System.out.println("Mal, repite: ");
            fondos = sc.nextDouble();
        }
        return fondos;
    }

    public static int leerUnidadGasto(){
        Scanner sc = new Scanner(System.in);
        int unidad;
        unidad = sc.nextInt();
        while(unidad<0){
            System.out.println("Mal, repite: ");
            unidad = sc.nextInt();
        }
        return unidad;
    }

    public static boolean leerVigencia(){
        Scanner sc = new Scanner(System.in);
        int v;
        v = sc.nextInt();
        while(v!=0 && v!=1){
            System.out.println("Mal, repite: ");
            v = sc.nextInt();
        }
        if(v==0)
            return false;
        else
            return true;
    }
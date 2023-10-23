#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
	// 1) Modificar esta sección
	const std::string optString = "t:f:h:--threads:--file:--help";
	const std::string opciones = "--threads N --file FILENAME [--help]";
	const std::string descripcion  = "Descripción:\n"
		                             "\t-t   Cantidad de threads a utilizar. Si es 1, entonces ejecuta la versión secuencial.\n"
									 "\t-f   Archivo a procesar.\n"
									 "\t-h   Muestra esta ayuda y termina.\n";
	typedef struct args_t{
		uint32_t numThreads;
		std::string filename;
		bool help;
	} args_t;
	
	// 2) Modificar constructor
	// 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
	// 4) Recuerde que para compilar nuevamente, y por tratarse
	//    de un archivo header, debe hacer primero un make clean
	args_t  parametros;
	int argc;
	char **argv;

	
public:
	checkArgs(int _argc , char **_argv); //Handler de argumentos
	int getThreads() const;
	std::string getFilename() const;
	bool showHelp() const; 
	void printUsage();
	
};

checkArgs::checkArgs(int _argc , char **_argv){
	
	parametros.numThreads = 1; // Valor por defecto
	parametros.filename  = "";
	parametros.help = false;

	argc = _argc;
	argv = _argv;
	
	if (argc == 1) {
        parametros.help = true;
        return;
    }


	std::string optString = "t:f:h";
		const option longOpts[] = {
			{"threads", required_argument, nullptr, 't'},
			{"file", required_argument, nullptr, 'f'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, no_argument, nullptr, 0}
		};

    int opt;
    while ((opt = getopt_long(argc, argv, optString.c_str(), longOpts, nullptr)) != -1) {
        switch (opt) {
            case 't':
                parametros.numThreads = std::stoi(optarg);
                break;
            case 'f':
                parametros.filename = optarg;
                break;
            case 'h':
                parametros.help = true;
                break;
            default:
				parametros.help = true;
                break;
        }
    }
}

void checkArgs::printUsage(){
	std::cout << "Uso: " <<
		argv[0] << " " << opciones << " " << descripcion << std::endl;
}

int checkArgs::getThreads() const{
	return(parametros.numThreads);
}

std::string checkArgs::getFilename() const{
	return(parametros.filename);
}

bool checkArgs::showHelp() const{
	return(parametros.help);
}




#endif

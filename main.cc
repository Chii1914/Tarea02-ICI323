#include <global.hh>
#include <checkArgs.hpp>


std::string removePunctuation(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(c)) {
            result += c;
        }
    }
    return(result);
}

// Función para dividir un texto en palabras
std::vector<std::string> splitText(const std::string &text) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(text);
    while (iss >> word) {
		
        //std::string lowercaseWord = word;
        for (char &c : word) {
            c = std::tolower(c);
        }
		
		word = removePunctuation(word);
        words.push_back(word);
    }
    return words;
}

void processLines(const std::vector<std::string>& lines, std::map<std::string, int>& histogram) {
    for(auto line : lines){
        // Dividir cada línea del texto en palabras
        std::vector<std::string> words = splitText(line);
        
        // Incrementar el conteo de cada palabra en el histograma
        for (const std::string &word : words) {
			histogram[word]++;
        }
    }
}

int main(int argc, char* argv[]) {

    checkArgs args(argc, argv);
    if(args.showHelp()){
        args.printUsage();
        return(EXIT_SUCCESS);
    }

    int numThreads = args.getThreads();
    std::string fileName = args.getFilename();

    if (fileName.empty()){
        std::cerr << "Debe proveer un nombre de archivo" << std::endl;
        return(EXIT_FAILURE);
    }
	std::vector<std::string> textInMemory;
    std::ifstream file(fileName); 
	
    if (!file) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return(EXIT_FAILURE);
    }
	
    std::string line;
    while (std::getline(file, line)) {
		textInMemory.push_back(line);
	}
	file.close();
	
    std::cout << "Procesando con " << numThreads << " threads..." << std::endl;
    std::map<std::string, int> wordHistogram;

    // Líneas por thread 
    int numLines = textInMemory.size();
    int LineasPorThread = numLines / numThreads;

    std::vector<std::thread> threads;

    std::mutex histogramMutex;

    for (int i = 0; i < numThreads; i++) {
        // Inicio y fin correspondiente a cada thread
        int Inicio = i * LineasPorThread;
        int Fin = (i == numThreads - 1) ? numLines : (i + 1) * LineasPorThread;

        // Función para procesar los datos de cada thread
        auto threadFunction = [Inicio, Fin, &textInMemory, &wordHistogram, &histogramMutex]() {
            std::map<std::string, int> threadHistogram;
            std::vector<std::string> threadLines(textInMemory.begin() + Inicio, textInMemory.begin() + Fin);
            processLines(threadLines, threadHistogram);

            //Unir al histograma global
            std::lock_guard<std::mutex> lock(histogramMutex);
            for (const auto &entry : threadHistogram) {
                wordHistogram[entry.first] += entry.second;
            }
        };

        //Crea al thread y lo emplea en la función para el histograma
        threads.emplace_back(threadFunction);
    }

    // Juntar los threads
    for (auto &thread : threads) {
        thread.join();
    }

    // Mostrar el histograma de palabras
    for (const auto &entry : wordHistogram) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }

    return(EXIT_SUCCESS);
}

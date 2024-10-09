#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void adjacencyMatrixToDot(int ** & adjMatrix, int size, const std::string& fileName, bool directed = false) {
    
    std::ofstream dotFile(fileName);

    if (!dotFile.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    // Write header for dot file
    if (directed)
        dotFile << "digraph G {" << std::endl;
    else
        dotFile << "graph G {" << std::endl;

    // Iterate over the adjacency matrix
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (adjMatrix[i][j] == 1) { // Edge exists between node i and node j
                if (directed)
                    dotFile << "    " << i << " -> " << j << ";" << std::endl;
                else if (i < j) // Avoid duplicate edges for undirected graphs
                    dotFile << "    " << i << " -- " << j << ";" << std::endl;
            }
        }
    }

    // Close graph
    dotFile << "}" << std::endl;

    dotFile.close();
    std::cout << "Graph has been written to " << fileName << std::endl;
}


void displayAdjacencyMatrix(int **v, int size) {
    // Displaying the matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}
void displayImgGraph(int **m, int size, string dotFile){
    adjacencyMatrixToDot(m, size, dotFile, false);
    system("dot -Tpng graphs.dot -o myGraph.png");
    system("nohup display myGraph.png &");
}
void visualizeGraph(int **m, int size, string dotFile, bool option=false){
    if(option){
        displayAdjacencyMatrix(m, size);
    }
    else{
        displayImgGraph(m, size, dotFile);
    }
}

void dealloc(int **v, int size) {
    for(int i = 0; i < size; i++)
        delete [] v[i];
    delete [] v;
}

void alloc(int ** &v, int size) {
    v = new int *[size];
    for(int i = 0; i < size; i++)
        v[i] = new int[size];
}

void readAdjMfromFile(string path, int **&m, int &t) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo\n";
        exit(EXIT_FAILURE);
    }
    
    file >> t;
    alloc(m, t);

    int i = 0, j = 0;
    while (file >> valor) {
        m[i][j] = valor;
        j++;
        if(j == t) {
            i++;
            j = 0;
        }
    }
    file.close();
}

void saveAdjM_infile(string path, int **m, int t) {
    ofstream file(path);
    if (!file.is_open()) {
        cout << "Error al abrir el archivo para guardar\n";
        exit(EXIT_FAILURE);
    }
    file << t << endl; // Save the size first
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            file << m[i][j] << " ";
        }
        file << endl;
    }
    file.close();
}

void addVertex(int **&m, int size, bool add_e = false){
    int **aux;
    int newSize = size + 1;
    alloc(aux, newSize);
    
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            aux[i][j] = m[i][j];
        }
    }
    dealloc(m, size);
    m = aux;
    if(add_e){
        addEdge(m, size);
    }
}
//review or improve
void removeVertex(int **&m, int size, int vertex) {
    if (vertex < 0 || vertex >= size) {
        cout << "Vertex out of range.\n";
        return;
    }

    int **aux;
    int newSize = size - 1;
    alloc(aux, newSize);

    // Copy over data excluding the vertex to be removed
    for (int i = 0, new_i = 0; i < size; i++) {
        if (i == vertex) continue; // Skip the vertex to remove
        for (int j = 0, new_j = 0; j < size; j++) {
            if (j == vertex) continue; // Skip the vertex to remove
            aux[new_i][new_j++] = m[i][j];
        }
        new_i++;
    }
    dealloc(m, size);
    m = aux;
}

void modify_v_ofTheGraph(int **&m, int t, ){

    while(opc != 3) {
    	cout << "\n"; 
	cout << "(1) Add vertice. \n";
    	cout << "(2) Remove vertice. \n";
        cout << "(3) exit. \n";
        cin >> opc;
        if (opc == 1) {
            addVertex(m, t, false);
        } 
        else if (opc == 2) {
	    int v;
	    cout << "Input vertice to remove: ";
	    cin >> v; 
            removeVertex(m, t, v);
        }
        else if (opc != 3) {
            cout << "Invalid option.\n";
        }
    }   

    
}

void modify_e_ofTheGraph(int **&m, int t){

    while(opc != 3) {
    	cout << "\n"; 
	cout << "(1) Add edge. \n";
    	cout << "(2) Remove edge. \n";
        cout << "(3) exit. \n";
        cin >> opc;
        if (opc == 1) {
            addEdge(m, t);
        } 
        else if (opc == 2) {
            removeEdge(m, t);
        }
        else if (opc != 3) {
            cout << "Invalid option.\n";
        }
    }
}
//verify for directed or not directed graph
void addEdge(int **m, int size) {
    int ni, nf;
    cout << "Initial node: "; cin >> ni;
    cout << "Final node: "; cin >> nf;
    if ((ni >= 1 && ni <= size) && (nf >= 1 && nf <= size)) {
        m[ni-1][nf-1] = 1;
        // If undirected, also add the reverse edge
        m[nf-1][ni-1] = 1;
        displayAdjacencyMatrix(m, size);
    } else {
        cout << "Nodes out of range.\n";
    }
}
//verify for directed graph
void removeEdge(int **m, int size) {
    int ni, nf;
    cout << "Initial node: "; cin >> ni;
    cout << "Final node: "; cin >> nf;
    if ((ni >= 1 && ni <= size) && (nf >= 1 && nf <= size)) {
        m[ni-1][nf-1] = 0;
        m[nf-1][ni-1] = 0; // If undirected, also remove the reverse edge
        displayAdjacencyMatrix(m, size);
    } else {
        cout << "Nodes out of range.\n";
    }
}
void modifyAdjacencyMatrix(int **&m, int t) {
    int opc = 0, ni, nf;
    while(opc != 3) {
        cout << "\n"; 
        cout << "(1) Modify vertice.\n";
        cout << "(2) Modify edge. \n";
        cout << "(3) exit. \n";
        cin >> opc;
        if (opc == 1) {
            modify_v_ofTheGraph(m, t);
        } 
        else if (opc == 2) {
            modify_e_ofTheGraph(m, t);
        }
        else if (opc != 3) {
            cout << "Invalid option.\n";
        }
    }
}

void createAdjacencyMatrix(int **&m, bool directed = false) {
    int v, e, ov, dv;
    cout << "Input number of vertices: "; cin >> v;
    cout << "Input number of edges: "; cin >> e;
    
    alloc(m, v);
    for (int i = 0; i < e; i++) {
        cout << "Edge " << (i + 1) << ": ";
        cout << "Origin vertex: " << "[1 to " << v << "]: "; cin >> ov;
        cout << "Destination vertex: " << "[1 to " << v << "]: "; cin >> dv;
        if ((ov >= 1 && ov <= v) && (dv >= 1 && dv <= v)) {
            m[ov-1][dv-1] = 1;
            if (!directed) {
                m[dv-1][ov-1] = 1; // Add reverse edge for undirected graph
            }
        } else {
            cout << "Vertices out of range" << endl;
        }
    }
}

void menu(){
    int **m;
    int t = 6; //for test
    int opc = 0, ni, nf;
    while(opc != 6) {
        cout << "\n"; 
        cout << "(1) Create adjacency matrix. \n";
        cout << "(2) Modify adjacency matrix vertex and aristas. \n";
        cout << "(3) Read adjacency matrix from file. \n";
        cout << "(4) Save adjacency matrix in file. \n";
        cout << "(5) visualize graph ----- two option\n";
        cout << "(6) Quit Program. \nopcion...: ";
        cin >> opc;  // editing this part, be careful
        if (opc == 1) {
            createAdjacencyMatrix(m, false);
        } 
        else if (opc == 2) {
            if(m != nullptr)
                modifyAdjacencyMatrix(m, t);
        }
        else if (opc == 3) {
            readAdjMfromFile("data.txt", m, t);
        }
        else if (opc == 4) {
            saveAdjM_infile("data.txt", m, t);
        }
        else if (opc == 5) {
            if(m != nullptr)
                visualizeGraph(m, t, "graphs.dot", true);
        }
        else if (opc != 6) {
            cout << "Invalid option.\n";
        }
    }
    dealloc(m, t);
    
}
int main() {
    menu();
    return 0;
}

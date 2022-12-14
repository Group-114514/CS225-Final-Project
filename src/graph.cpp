#include "graph.h"
#include "utils.h"


/////////////////////////////////
// constructor //
/////////////////
void Graph_directed::create_helper(vector<inputVertex> input_node, vector<Edge> input_edge) {
    for (inputVertex v : input_node) {
        Vertex topush;
        topush.name_ = v[0];
        topush.other_data = v;
        graph.insert(pair<string, Vertex>(v[0], topush)); //initilize graph
        nodes.push_back(v[0]); // pushed all nodes
    }
    // all nodes inserted
    for (Edge e : input_edge) {
        string from_id = e.from[0];
        string to_id = e.to[0];
        double weight = e.weight;
        // all three data fetched
        insert_in_order(graph[from_id].neighbors_, pair<string, double>(to_id, weight));
        graph[from_id].num_of_neighbor++;
        graph[to_id].num_as_dest++;
    }
}

Graph_directed::Graph_directed(const std::string & vertex_file, const std::string & edge_file) {
    vector<inputVertex> i_node;
    vector<Edge> i_edge;
    string n_string = file_to_string(vertex_file);
    string e_string = file_to_string(edge_file);

    vector<string> line_node;
    SplitString(n_string, '\n', line_node);
    vector<string> line_edge;
    SplitString(e_string, '\n', line_edge);

    for (string s : line_node) {
        inputVertex iv;
        SplitString(s, ',', iv);
        if (iv[0].empty()) continue; // empty input
        i_node.push_back(iv);
    }
    for (string s : line_edge) {
        inputVertex ivfrom;
        inputVertex ivto;
        vector<string> temp;
        SplitString(s, ',', temp);
        if (temp.size() <= 2) {
            // invalid line
            continue;
        }
        ivfrom.push_back(temp[0]);
        ivto.push_back(temp[1]);
        double wei = std::stod(temp[2]);
        Edge edge__;
        edge__.from = ivfrom;
        edge__.to = ivto;
        edge__.weight = wei;
        i_edge.push_back(edge__);
    }
    create_helper(i_node,i_edge);
}


Graph_directed::Graph_directed(const std::string & vertex_file, const std::string & edge_file, const std::string & bc_file) {
    vector<inputVertex> i_node;
    vector<Edge> i_edge;
    string n_string = file_to_string(vertex_file);
    string e_string = file_to_string(edge_file);

    vector<string> line_node;
    SplitString(n_string, '\n', line_node);
    vector<string> line_edge;
    SplitString(e_string, '\n', line_edge);

    for (string s : line_node) {
        inputVertex iv;
        SplitString(s, ',', iv);
        if (iv[0].empty()) continue; // empty input
        i_node.push_back(iv);
    }
    for (string s : line_edge) {
        inputVertex ivfrom;
        inputVertex ivto;
        vector<string> temp;
        SplitString(s, ',', temp);
        if (temp.size() <= 2) {
            // invalid line
            continue;
        }
        ivfrom.push_back(temp[0]);
        ivto.push_back(temp[1]);
        double wei = std::stod(temp[2]);
        Edge edge__;
        edge__.from = ivfrom;
        edge__.to = ivto;
        edge__.weight = wei;
        i_edge.push_back(edge__);
    }
    create_helper(i_node,i_edge);

    std::ifstream myfile(bc_file);
    if (myfile.is_open()) {
        std::string line;
        while (std::getline(myfile, line)) {
            auto id = line.substr(0, 3);
            auto bc = std::stod(line.substr(4));
            set_bc(id, bc);
        }
    } else 
        throw std::runtime_error("File cannot open: " + bc_file);
    myfile.close();
}

///////////////////////////////////////
// function  private //

/**
 * should insert the element in the ascending order;
*/
void Graph_directed::insert_in_order(vector<pair<string, double>>& toinsert, pair<string, double> data) {
    //
    toinsert.insert(
        // upper bound and lower bound have no influence
        std::upper_bound(toinsert.begin(), toinsert.end(), data, sortbysec),
        data
    );
}



///////////////////////////////////////
// function  public //

vector<pair<string, double>> Graph_directed::get_neighbors(const string& id) const {
    if (graph.find(id) == graph.end()) {
        vector<pair<string, double>> toreturn;
        toreturn.push_back(pair<string, double>("not exist", -1));
        return toreturn;
    }
    return graph.at(id).neighbors_;
}

pair<string, double> Graph_directed::get_ith_closest_neighbor(const string& id,const unsigned i) const {
    if (i >= graph.at(id).num_of_neighbor) return pair<string, double>("not exist", -1);
    return graph.at(id).neighbors_[i];
}

unsigned Graph_directed::get_num_of_neighbors(const string& id) const {
    if (graph.find(id) == graph.end()) {
        return 0;
    }
    return graph.at(id).num_of_neighbor;
}

vector<string> Graph_directed::getAll_vertex() const {
    return nodes;
}

vector<string> Graph_directed::bfs_one_component(const string& id) const {
    queue<string> qu;
    unordered_map<string,bool> visited; // use map is for bfs whole graph to copy;
    qu.push(id);
    vector<string> toreturn;
    while(!qu.empty()) {
        // bfs start
        vector<string> rim;
        while(!qu.empty()) {
            rim.push_back(qu.front());
            qu.pop();
        }

        // rim fetched
        for (const string& s : rim) {
            if (visited.find(s) != visited.end()) continue; // != end means already existed, should continue;

            visited.insert(pair<string,bool>(s,true)); // now map visisted
            toreturn.push_back(s);

            for (const pair<string, double>& p : get_neighbors(s)) {
                // now get all neighbors
                const string& neigh_s = p.first;
                if (visited.find(neigh_s) == visited.end()) qu.push(neigh_s); // == end means haven't visited, should be added to the queue
            }
        }
        // now bfs completed;
    }
    return toreturn;
}


// have problem for it is an directed graph
vector<vector<string>> Graph_directed::bfs_whole_graph(const string& id) const {
    queue<string> qu;
    unordered_map<string,bool> visited; // use map is for bfs whole graph to copy;
    set<string> unvisited;
    for (string s : nodes) {
        unvisited.insert(s);
    }
    //
    qu.push(id);
    vector<vector<string>> toreturn;
    vector<string> toreturn_one_component;
    //
    while(!qu.empty() && !unvisited.empty()) {
        // bfs start
        vector<string> rim;
        while(!qu.empty()) {
            rim.push_back(qu.front());
            qu.pop();
        }

        // rim fetched
        for (const string& s : rim) {
            if (visited.find(s) != visited.end()) continue; // != end means already existed, should continue;

            visited.insert(pair<string,bool>(s,true)); // now map visisted
            unvisited.erase(s);
            toreturn_one_component.push_back(s);

            for (const pair<string, double>& p : get_neighbors(s)) {
                // now get all neighbors
                const string& neigh_s = p.first;
                if (visited.find(neigh_s) == visited.end()) qu.push(neigh_s); // == end means haven't visited, should be added to the queue
            }
        }
        // now bfs_one component completed;

        if (qu.empty() && !unvisited.empty()) {
            // now to return insert one component
            toreturn.push_back(toreturn_one_component);
            qu.push(*unvisited.begin());
            toreturn_one_component.clear();
        }
    }
    return toreturn;
}

shortestPathTable Graph_directed::gen_shortest_path_table() const {
    auto vertexs = getAll_vertex();
    double INF = __DBL_MAX__;
    unordered_map<string, unordered_map<string, double>> adj;
    unordered_map<string, unordered_map<string, double>> path;
    unordered_map<string, unordered_map<string, string>> path_iter;

    int c = 1;
    // isolated note container
    std::unordered_set<string> ignored_vertex;
    std::unordered_set<string> cleaned_vertex;
    for (auto vertex : vertexs) {
        std::cout << "cleaning: " << c++ << '/' << vertexs.size() << std::endl;
        if (get_num_of_neighbors(vertex) == 0 && get_num_as_dest(vertex) == 0) {
            ignored_vertex.insert(vertex);
        }
        else { 
            cleaned_vertex.insert(vertex);
        }
    }

    c = 1;
    // initilize all the matrices
    for (auto vertex : cleaned_vertex) {
        std::cout << "initilize: " << c++ << '/' << cleaned_vertex.size() << std::endl;
        auto neighbors = get_neighbors(vertex);
        for (auto vertex_dest : cleaned_vertex) {
            adj[vertex][vertex_dest] = INF;
            path[vertex][vertex_dest] = INF;
            path_iter[vertex][vertex_dest] = "";
        }
        for (auto neighbor : neighbors) {
            if (cleaned_vertex.find(neighbor.first) == cleaned_vertex.end())
                continue;
            adj[vertex][neighbor.first] = neighbor.second;
            path[vertex][neighbor.first] = neighbor.second;
            path_iter[vertex][neighbor.first] = neighbor.first;
        }
    }

    c = 1;
    // floyd Warshall update path and path_iter
    std::cout << "----------" <<
                 "\nignored: " << ignored_vertex.size() <<
                 "\ncleaned: " << cleaned_vertex.size() << 
                 "\n---------"   << std::endl;
    for (auto k : cleaned_vertex) {
        std::cout << "floyd Warshall: " << c++ << '/' << cleaned_vertex.size() << std::endl;
        for (auto i : cleaned_vertex) {
            for (auto j : cleaned_vertex) {
                // no need update if one path is infinite
                if (path[i][k] == INF) 
                    continue;
                if (path[i][j] > path[i][k] + path[k][j]) {
                    path[i][j] = path[i][k] + path[k][j];
                    path_iter[i][j] = path_iter[i][k];
                }
            }
        }
    }

    c = 1;
    // make the output
    shortestPathTable out;
    for (auto vertex : vertexs) {
        std::cout << "make output: " << c++ << '/' << vertexs.size() << std::endl;
        for (auto vertex_dest : vertexs) {
            // handle ignored vertex
            if (ignored_vertex.find(vertex) != ignored_vertex.end()) 
                out[vertex][vertex_dest] = {std::vector<string>(), INF};
        
            auto shortest_path = fetch_path_from_path_iter(path_iter, vertex, vertex_dest);
            auto shortest_weight = path[vertex][vertex_dest];
            out[vertex][vertex_dest] = {shortest_path, shortest_weight};
        }       
    }
    return out;
}

vector<string> Graph_directed::fetch_path_from_path_iter(unordered_map<string, unordered_map<string, string>>& path_iter, string& v1, string& v2) const {
    std::vector<string> out;
    if (path_iter[v1][v2] == "" || v1 == v2) return {};
    auto curr = v1;
    while (true) {
        curr = path_iter[curr][v2];
        out.push_back(curr);
        if (curr == v2)
            break;
    }
    return out;
}

std::vector<string> Graph_directed::get_shortest_path(shortestPathTable& sp_table, const string& id1, const string& id2) const {
    return sp_table.at(id1).at(id2).first;
}

double Graph_directed::get_shortest_weight(shortestPathTable& sp_table, const string& id1, const string& id2) const {
    auto weight = sp_table.at(id1).at(id2).second;
    return weight == __DBL_MAX__ ? -1 : weight;
}

void Graph_directed::UpdateBC() {
    shortestPathTable spt = gen_shortest_path_table();
    for (string start : nodes) {
        if (get_num_of_neighbors(start) == 0) continue; // no neighbor;
        for (string end : nodes) {
            if(start == end) continue;
            if(get_num_as_dest(end) == 0) continue;  // cannot be destination;
            const vector<string>& path = get_shortest_path(spt, start, end);
            for (string s : path) {
                if (s == start || s == end) continue;
                set_bc(s, get_bc(s) + 1);
            }
        }
    }
}

void Graph_directed::dump_bc_to_csv(const string file) {
    std::ofstream myfile;
    myfile.open (file);

    if(myfile.is_open()) {  
        for (auto vertex : nodes) {
            string str; 
            str += vertex;
            str += ',';
            str += std::to_string(get_bc(vertex));
            myfile << str << std::endl;
        }
        myfile.close();
    } else 
        throw std::runtime_error("File cannot open: " + file);
    std::cout << "successfully dump to: " + file << std::endl;
}

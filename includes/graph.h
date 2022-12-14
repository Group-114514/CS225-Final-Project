#pragma once
#include "include.hpp"

using std::string;
using std::set;
using std::vector;
using std::pair;
using std::list;
using std::map;
using std::unordered_map;
using std::queue;
using std::unordered_set;

//template<class T>
 
typedef vector<string> inputVertex;
typedef std::unordered_map<string, std::unordered_map<string, std::pair<std::vector<string>, double>>> shortestPathTable;


class Graph_directed{
    private:
        ////////////////////////////
        // private struct //
        struct Edge {
            inputVertex from;
            inputVertex to;
            double weight = 0;
        };

        class Vertex {
            public:
            string name_ = "-1"; // id of the air port
            vector<string> other_data;
            // store the neighbor index and the corresponding weight
            vector<pair<string, double>> neighbors_;
            unsigned num_of_neighbor = 0;
            unsigned num_as_dest = 0;
            double bc_weight = 0;

            bool operator==(Vertex& v) {
                return v.name_ == this->name_;
            };

            bool operator!=(Vertex& v) {
                return v.name_ != this->name_;
            };

            Vertex operator=(Vertex& v) {
                name_ = v.name_;
                other_data = v.other_data;
                neighbors_ = v.neighbors_;
                num_of_neighbor = v.num_of_neighbor;
                bc_weight = v.bc_weight;
                num_as_dest = v.num_as_dest;
                return *this;
            };

            Vertex (const Vertex& v) {
                name_ = v.name_;
                other_data = v.other_data;
                neighbors_ = v.neighbors_;
                num_of_neighbor = v.num_of_neighbor;
                bc_weight = v.bc_weight;
                num_as_dest = v.num_as_dest;
            };

            Vertex(){}

        };

        ///////////////////////////
        // private data //
        unordered_map<string,Vertex> graph;
        set<Edge> all_edge;
        vector<string> nodes;

        ///////////////////////////
        // helper function //
        
        /* funtion used for set neighbor */
        void insert_in_order(vector<pair<string, double>>&, pair<string, double>);
        
        /* function used to support insert in order */
        static bool sortbysec(const pair<string,double> &a,
                const pair<string,double> &b) {
            return (a.second < b.second);
        }

        /**
         * @param id the id of the airport
         * @param bs the bs of the airport to be set
        */
        void set_bc(string& id, double bc) {
            graph[id].bc_weight = bc;
        }

        /**
         * initilize the whole graph with node and edge as input
         * 
         * @param input_node input of vertex, this mp should be array<string>, Vertex[0] is airfield name
         * @param input_edge input of edge, including weight
        */
        void create_helper(vector<inputVertex>, vector<Edge>);

        /**
         * helper for gen_shortest_path_table(), fetch shortest path from path_iter matrix
         * 
         * @param path_iter path iter matrix
         * @param v1 start vertex
         * @param v2 dest vertex
         * @return shortest path vertex
        */
        std::vector<string> fetch_path_from_path_iter(std::unordered_map<string, std::unordered_map<string, string>>& path_iter, string& v1, string& v2) const;

        /// @brief start of public function

    public:
        ////////////////////////////
        // constructor //
        Graph_directed();

        /**
         * initilize the whole graph with node and edge as input
         * 
         * @param vertex_file input of vertex_file, this should be csv
         * @param edge_file input of edge_file, should be csv
        */
        Graph_directed(const std::string & vertex_file, const std::string & edge_file);

        /**
         * initilize the whole graph with node and bc as input
         * 
         * @param vertex_file input of vertex_file, this should be csv
         * @param edge_file input of edge_file, should be csv
         * @param bc_file input of bc_file, should be csv
        */
        Graph_directed(const std::string & vertex_file, const std::string & edge_file, const std::string & bc_file);

        ////////////////////////////
        // function //
        /**
         * @param id the id of the air port
         * @return vector<string> the vector of string containing the air port name and its neighbor
        */
        vector<pair<string, double>> get_neighbors(const string& id) const;

        /**
         * @param id the id of the air port
         * @param i the ith' closest of the air port
         * @return vector<string> the vector of string containing the air port name and its neighbor
        */
        pair<string, double> get_ith_closest_neighbor(const string& id,const unsigned i) const;

        /**
         * @param id the id of the air port
         * @return number of neighbors airfield this airfield have
        */
        unsigned get_num_of_neighbors(const string& id) const;

        unsigned get_num_as_dest(const string& id) const{
            if (graph.find(id) == graph.end()) {
                return 0;
            }
            return graph.at(id).num_as_dest;
        }


        // return all vertex
        vector<string> getAll_vertex() const;

        /**
         * @param id the id of the airport
         * @return bs the bs of the airport toreturn
        */
        double get_bc(string& id) {
            return graph[id].bc_weight;
        }

        /**
         * @param id the id of the air port
         * @return all vertex in component in visiting order
        */
        vector<string> bfs_one_component(const string& id) const;

        /**
         * @param id the id of the air port
         * @return all vertex in component in visiting order
        */
        vector<vector<string>> bfs_whole_graph(const string& id) const;

        /**
         * @return table used to fetch shortest path
        */
        shortestPathTable gen_shortest_path_table() const;

        /**
         * @param sp_table shortest path table
         * @param id1 the id of the start air port
         * @param id2 the id of the dest air port
         * @return shortest path from id1 to id2
        */
        std::vector<string> get_shortest_path(shortestPathTable& sp_table, const string& id1, const string& id2) const;

        /**
         * @param sp_table shortest path table
         * @param id1 the id of the start air port
         * @param id2 the id of the dest air port
         * @return shortest distance from id1 to id2, return -1 if no valid path
        */
        double get_shortest_weight(shortestPathTable& sp_table, const string& id1, const string& id2) const;


        /**
         * @param id the id of the air port
         * @return all related data in the vertex, v[0] is the same as id; if not found return empty;
         * 
        */
        vector<string> get_vertex_other_data(const string id) const {
            if (graph.find(id) == graph.end()) {
                return vector<string>();
            }
                return graph.at(id).other_data;
        }

        /**
         * @param file the file to be writen
         * 
        */
        void dump_bc_to_csv(const string file);

        void UpdateBC();
};
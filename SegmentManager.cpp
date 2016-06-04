#include "SegmentManager.h"
#define WIDTH 4

SegmentManager::SegmentManager()
{
    
}

double blurWidth(const double& sigma)
{
    return ceil(sigma * 4) + 1;
}

double diff(const Image& image, const int& i1, const int& j1, const int& i2, const int& j2)
{
    return sqrt(pow(image.pixelColor(i2, j2).redQuantum() - image.pixelColor(i1, j1).redQuantum(), 2) +
                pow(image.pixelColor(i2, j2).greenQuantum() - image.pixelColor(i1, j1).greenQuantum(), 2) +
                pow(image.pixelColor(i2, j2).blueQuantum() - image.pixelColor(i1, j1).blueQuantum(), 2));
}

bool comparator(const Edge& e1, const Edge& e2)
{
    return e1.weight < e2.weight;
}

void buildGraph(std::vector<Edge>& edges, const Image& image)
{
    int e = 0;
    int w = (int)image.columns();
    int h = (int)image.rows();
    edges.reserve(w * h * 4);
    
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            
            if (i < w-1) {
                edges.push_back(Edge());
                edges[e].a = j * w + i;
                edges[e].b = j * w + i + 1;
                edges[e].weight = diff(image, i, j, i+1, j);
                e++;
            }
            
            if (j < h-1) {
                edges.push_back(Edge());
                edges[e].a = j * w + i;
                edges[e].b = (j + 1) * w + i;
                edges[e].weight = diff(image, i, j, i, j+1);
                e++;
            }
            
            if (i < w-1 && j < h-1) {
                edges.push_back(Edge());
                edges[e].a = j * w + i;
                edges[e].b = (j + 1) * w + i + 1;
                edges[e].weight = diff(image, i, j, i+1, j+1);
                e++;
            }
            
            if (i < w-1 && j > 0) {
                edges.push_back(Edge());
                edges[e].a = j * w + i;
                edges[e].b = (j - 1) * w + i + 1;
                edges[e].weight = diff(image, i, j, i+1, j-1);
                e++;
            }
        }
    }
    
    std::sort(edges.begin(), edges.end(), comparator);
}

double avgWeight(std::vector<Edge>& edges)
{
    double w = 0;
    for (size_t i = 0; i < edges.size(); i++) {
        w += edges[i].weight;
    }
    
    return w / edges.size();
}

void segmentGraph(Forest& forest, std::vector<Edge>& edges, const double& k)
{
    std::vector<double> threshold(forest.numSets(), k);
    for (size_t i = 0; i < edges.size(); i++) {
        const Edge& e(edges[i]);
        
        int a = forest.find(e.a);
        int b = forest.find(e.b);
        
        if (a != b) {
            if (e.weight <= threshold[a] && e.weight <= threshold[b]) {
                forest.join(a, b);
                a = forest.find(a);
                threshold[a] = e.weight + k / forest.nodeSize(a);
            }
        }
    }
}

void joinSmallComponents(Forest& forest, std::vector<Edge>& edges)
{
    const int minSize = forest.numSets() / 10;
    
    for (size_t i = 0; i < edges.size(); i++) {
        const Edge& e(edges[i]);
        
        int a = forest.find(e.a);
        int b = forest.find(e.b);
        
        if (a != b) {
            if (forest.nodeSize(a) < minSize || forest.nodeSize(b) < minSize) {
                forest.join(a, b);
            }
        }
    }
}

double randNum()
{
    return ((double) rand() / (RAND_MAX));
}

void assignColors(Image& image, Forest& forest)
{
    int w = (int)image.columns();
    int h = (int)image.rows();
    std::vector<ColorRGB> colors(w*h);
    for (size_t i = 0; i < colors.size(); i++) {
        colors[i] = ColorRGB(randNum(), randNum(), randNum());
    }
    
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            int index = forest.find(j * w + i);
            image.pixelColor(i, j, colors[index]);
        }
    }
}

int SegmentManager::segment(Image& output, const Image& input, const double& sigma, const double& k)
{
    output = input;
    Forest forest(int(input.columns()*input.rows()));
    std::vector<Edge> edges;
    
    output.gaussianBlur(blurWidth(sigma), sigma);
    buildGraph(edges, output);
    segmentGraph(forest, edges, k);
    joinSmallComponents(forest, edges);
    assignColors(output, forest);
    
    return forest.numSets();
}
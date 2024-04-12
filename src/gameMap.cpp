#include "gameMap.h"
#include "math.h"

bool GameMap::loadGameMap(const char* filename) {
    using json = nlohmann::json;
    std::ifstream f(filename);

    if (!f.good()) {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }
    json jData = json::parse(f);

    int w = jData["width"];
    int h = jData["height"];
    int numLayers = jData["layers"].size();

    GameMap* map = new GameMap(w, h);
    // Allocate memory each layer
    map->layers = new sLayer[numLayers];

    for (int l = 0; l < numLayers; ++l) {
        //Allocate memory for data inside each layer
        map->layers[l].data = new sCell[w*h];
        json layer = jData["layers"][l];
        for (int x = 0; x < map->width; ++x) {
            for (int y = 0; y < map->height; ++y) {
                int index = x + y*map->width;
                int type = layer["data"][index].get<int>();
                map->getCell(x, y, l).type = eCellType(type-1);
            }
        }
    }

    this->width   = map->width;
    this->height  = map->height;
    this->data    = map->data;
    this->layers  = map->layers;
    this->nLayers = numLayers;

    delete map;

    std::cout << "Game Map loaded correctly: " << filename << std::endl;

    return true;
}

void GameMap::drawMap(Image& framebuffer, const Image& tileset, const Vector2& pPosition, const Camera& camera) {
    //Size in pixels of a cell
    int numtiles = tileset.width/16;
    int cs = tileset.width / numtiles;

    for (int l = 0; l < this->nLayers; ++l)
    for (int x = 0; x < this->width  ; ++x)
    for (int y = 0; y < this->height ; ++y)    {
        //Get cell info
        sCell& cell = this->getCell(x, y, l);

        if (cell.type == -1) continue;

        int type = (int) cell.type;

        //Compute tile pos in tileset image
        int tilex = (type%numtiles) * cs;
        int tiley = floor(type/numtiles) * cs;

        //Create tile area
        Area area(tilex, tiley, cs, cs);

        //Place offset here if you want
        int screenx = x * cs - camera.position.x + camera.half.x;
        int screeny = y * cs - camera.position.y + camera.half.y;

        //Avoid rendering out of screen stuff
        if (screenx < -cs || screenx > (int) framebuffer.width ||
            screeny < -cs || screeny > (int) framebuffer.height)
            continue;

        //Draw region of tileset inside framebuffer
        framebuffer.drawImage(tileset, screenx, screeny, area);
    }
} 
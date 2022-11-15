

struct TMXProperty
{
	char name[30];
	char type[30];
	char value[30];
	int intValue;
	float floatValue;
	bool boolValue;
	char fileValue[30];
	char stringValue[30];
};

struct TMXImage
{
	char format[30];
	char source[30];
	char trans[30];
	int width, height;
	char* data;
};

struct TMXObject
{
	int id;
	char name[30];
	Vector2D pos;
	int width, height;
	char type[30]; //point ellipse rect etc;
	float rotation;
	int gid;
	bool visible;
	int typeID;
	vector<TMXProperty> properties;
};

struct TMXObjectGroup
{
	int id;
	char name[30];
	char color[30];
	float opacity;
	bool visible;
	bool locked;
	int offsetX, offsetY;
	char drawOrder[30];
	vector<TMXObject> objects;
	vector<TMXProperty> properties;
};

struct TMXImageLayer
{
	int id;
	char name[30];
	int offsetX, offsetY;
	float opacity;
	bool visible;
	vector<TMXProperty> properties;
	TMXImage image;
};

struct TMXLayer
{
	int id;
	char name[30];
	char dataEncoding[30];
	uint32_t width, height;
	float opacity;
	bool visible;
	bool locked;
	uint32_t offsetX, offsetY;
	vector<TMXProperty> properties;
	uint32_t* data;
};

struct TMXGroup
{
	int id;
	char name[30];
	int offsetX, offsetY;
	float opacity;
	bool visible;
	vector<TMXProperty> properties;
	vector<TMXLayer> layers;
	vector<TMXImageLayer> imageLayers;
	vector<TMXObjectGroup> objectGroups;
	vector<TMXGroup> groups;
};



struct TMXTile
{
	int id;
	int typeID;
	vector<TMXProperty> properties;
};

struct TMXTileSet
{
	int firstGID;
	char name[30];
	unsigned int tileWidth, tileHeight;
	int spacing;
	unsigned int tileCount;
	unsigned int columns;
	char source[30];
	unsigned int sourceHeight, sourceWidth;
	vector<TMXTile> tiles;
	vector<TMXProperty> properties;
	TMXImage image;
	int tileSetID;
	//grid
	int tileOffsetX, tileOffsetY;
	//terraintypes
	//wangsets
	//image
};

struct TMXMap
{
	float version;
	char tiledVersion[30];
	char orientation[30];
	char renderOrder[30];
	uint32_t width, height;
	uint32_t tileWidth, tileHeight;
	bool infinite;
	char backGroundColor[30];
	unsigned int nextLayerID, nextObjectID; 
	vector<TMXProperty> properties;
	vector<TMXTileSet> tileSets;
	vector<TMXGroup> groups;
	vector<TMXLayer> layers;
	vector<TMXImageLayer> imageLayers;
	vector<TMXObjectGroup> objectGroups;
};
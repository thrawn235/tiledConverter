#include <vector>
#include <stdio.h>
#include <iostream>
//#include <conio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <string>

using namespace std;

#include "Vector2D.h"
#include "TMXStructs.h"

//evil ;)
bool writeSignature = false;
bool writeMapInfo = false;
bool writeLayerInfo = false;
bool writeObjects = false;
unsigned int numLayersToWrite = 0;
unsigned char metaByte = 0;
bool writeMetaByte = false;

struct XMLValuePair
{
    char name[30];
    char value[30];
};

struct XMLLine
{
    char                    firstChar;
    char                    tag[30];
    vector<XMLValuePair>    pairs;
    bool                    closing;
};


void PrintXMLLine( XMLLine line )
{
    printf("<%c%s ", line.firstChar, line.tag );
    for( unsigned int i = 0; i < line.pairs.size(); i++ )
    {
        printf( "%s=\"%s\" ", line.pairs[i].name, line.pairs[i].value );
    }
    printf( "> closing=%i\n", line.closing );
}

XMLLine ExtractXMLLine( FILE* file )
{
    XMLLine line;
    line.firstChar = ' ';
    //find first <
    while( line.firstChar != '<' )
    {
        fscanf( file, "%c", &line.firstChar );
        //printf("prechar:%c\n", line.firstChar );
    }
    


    //first character in line
    fscanf( file, "%c", &line.firstChar );
    //printf("fistchar:%c\n", line.firstChar );
    if( line.firstChar == '?' )
    {
        
    }
    else if( line.firstChar == '/' )
    {
        line.closing = true;
    }
    else
    {
        line.closing = false;
        //printf("back one..." );
        line.firstChar = ' ';
        fseek( file, -1, SEEK_CUR );
    }

    fscanf( file, "%[^ >]", line.tag );
    //printf( "tag:%s", line.tag );

    XMLValuePair pair;
    char whitespace = ' ';
    fscanf( file, "%c", &whitespace );  
    if( whitespace == '>' )
    {
        fscanf( file, "\n" );
        return line;
    }
    //fseek( file, -1, SEEK_CUR );
    
    bool done = false;
    while( !done )
    {
        //printf("test\n");
        fscanf( file, "%[^=]=", pair.name );
        fscanf( file, "\"%[^\"]\"", pair.value );
        line.pairs.push_back( pair );
        fscanf( file, "%c", &whitespace );
        if( whitespace == '?' )
        {
            fscanf( file, ">\n" );
            done = true;
            line.closing = true;
        }
        else if( whitespace == '/' )
        {
            fscanf( file, ">\n" );
            line.closing = true;
            done = true;
        }
        else if( whitespace == '>' )
        {
            fscanf( file, "\n" );
            done =  true;
            line.closing = false;
        }
    }
    return line;
}

char* GetValue( XMLLine line, char* name )
{
    for( unsigned int i = 0; i < line.pairs.size(); i++ )
    {
        if( strcmp( name, line.pairs[i].name ) == 0 )
        {
            //printf("name:%s value:%s\n", line.pairs[i].name, line.pairs[i].value );
            return line.pairs[i].value;
        }
    }
    return NULL;
}


void PopulateTMXMap( XMLLine line, TMXMap* tmxMap )
{
    if( GetValue( line, "version") != NULL )
    {
        tmxMap->version = atof( GetValue( line, "version") );
    }
    if( GetValue( line, "tiledversion") != NULL )
    {
        strcpy( tmxMap->tiledVersion, GetValue( line, "tiledversion") );
    }
    if( GetValue( line, "orientation") != NULL )
    {
        strcpy( tmxMap->orientation, GetValue( line, "orientation") );
    }
    if( GetValue( line, "renderorder") != NULL )
    {
        strcpy( tmxMap->renderOrder, GetValue( line, "renderorder") );
    }
    if( GetValue( line, "width") != NULL )
    {
        tmxMap->width = atoi( GetValue( line, "width") );
    }
    if( GetValue( line, "height") != NULL )
    {
        tmxMap->height = atoi( GetValue( line, "height") );
    }
    if( GetValue( line, "tilewidth") != NULL )
    {
        tmxMap->tileWidth = atoi( GetValue( line, "tilewidth") );
    }
    if( GetValue( line, "tileheight") != NULL )
    {
        tmxMap->tileHeight = atoi( GetValue( line, "tileheight") );
    }
    if( GetValue( line, "infinite") != NULL )
    {
        tmxMap->infinite = atoi( GetValue( line, "infinite") );
    }
    if( GetValue( line, "renderorder") != NULL )
    {
        strcpy( tmxMap->backGroundColor, GetValue( line, "renderorder") );
    }
    if( GetValue( line, "backgroundcolor") != NULL )
    {
        strcpy( tmxMap->backGroundColor, GetValue( line, "backgroundcolor") );
    }
    if( GetValue( line, "nextlayerid") != NULL )
    {
        tmxMap->nextLayerID = atoi( GetValue( line, "nextlayerid") );
    }
    if( GetValue( line, "nextobjectid") != NULL )
    {
        tmxMap->nextObjectID = atoi( GetValue( line, "nextobjectid") );
    }
}

void PrintTMXMapValues( TMXMap map )
{
    printf( "version        = %f \n", map.version       );
    printf( "tiledVersion   = %s \n", map.tiledVersion );
    printf( "orientation    = %s \n", map.orientation   );
    printf( "renderOrder    = %s \n", map.renderOrder   );
    printf( "width          = %i \n", map.width         );
    printf( "height         = %i \n", map.height        );
    printf( "tileWidth      = %i \n", map.tileWidth     );
    printf( "tileheight     = %i \n", map.tileHeight    );
    printf( "infinite       = %i \n", map.infinite      );
    printf( "nextLayerID    = %i \n", map.nextLayerID   );
    printf( "nextObjectID   = %i \n", map.nextObjectID );
}

void PopulateTMXLayer( XMLLine line, TMXLayer* layer )
{
    if( GetValue( line, "id") != NULL )
    {
        layer->id = atoi( GetValue( line, "id") );
    }
    if( GetValue( line, "name") != NULL )
    {
        strcpy( layer->name, GetValue( line, "name") );
    }
    if( GetValue( line, "dataencoding") != NULL )
    {
        strcpy( layer->dataEncoding, GetValue( line, "dataencoding") );
    }
    if( GetValue( line, "width") != NULL )
    {
        layer->width = atoi( GetValue( line, "width") );
    }
    if( GetValue( line, "height") != NULL )
    {
        layer->height = atoi( GetValue( line, "height") );
    }
    if( GetValue( line, "opacity") != NULL )
    {
        layer->opacity = atoi( GetValue( line, "opacity") );
    }
    else
    {
       layer->opacity = 1; 
    }
    if( GetValue( line, "visible") != NULL )
    {
        layer->visible = atoi( GetValue( line, "visible") );
    }
    else
    {
        layer->visible = 1;
    }
    if( GetValue( line, "locked") != NULL )
    {
        layer->locked = atoi( GetValue( line, "locked") );
    }
    else
    {
        layer->locked = 0;
    }
    if( GetValue( line, "offsetx") != NULL )
    {
        layer->offsetX = atoi( GetValue( line, "offsetx") );
    }
    else
    {
        layer->offsetX = 0;
    }
    if( GetValue( line, "offsety") != NULL )
    {
        layer->offsetY = atoi( GetValue( line, "offsety") );
    }
    else
    {
        layer->offsetY = 0;
    }
}

void PrintTMXLayerValues( TMXLayer layer )
{
    printf( "       id          = %i \n", layer.id      );
    printf( "       name        = %s \n", layer.name        );
    printf( "       Width       = %i \n", layer.width   );
    printf( "       Height      = %i \n", layer.height  );
    printf( "       visible     = %i \n", layer.visible     );
    printf( "       locked      = %i \n", layer.locked  );
    printf( "       offsetX     = %i \n", layer.offsetX     );
    printf( "       offsetY     = %i \n", layer.offsetY     );
}

void PopulateTMXTileSet( XMLLine line, TMXTileSet* tileSet )
{
    if( GetValue( line, "firstgid") != NULL )
    {
        tileSet->firstGID = atoi( GetValue( line, "firstgid") );
    }
    if( GetValue( line, "name") != NULL )
    {
        strcpy( tileSet->name, GetValue( line, "name") );
    }
    if( GetValue( line, "tilewidth") != NULL )
    {
        tileSet->tileWidth = atoi( GetValue( line, "tilewidth") );
    }
    else
    {
        tileSet->tileWidth = 0;
    }
    if( GetValue( line, "tileheight") != NULL )
    {
        tileSet->tileHeight = atoi( GetValue( line, "tileheight") );
    }
    else
    {
        tileSet->tileHeight = 0;
    }
    if( GetValue( line, "spacing") != NULL )
    {
        tileSet->spacing = atoi( GetValue( line, "spacing") );
    }
    else
    {
        tileSet->spacing = 0;
    }
    if( GetValue( line, "tilecount") != NULL )
    {
        tileSet->tileCount = atoi( GetValue( line, "tilecount") );
    }
    else
    {
        tileSet->tileCount = 0;
    }
    if( GetValue( line, "columns") != NULL )
    {
        tileSet->columns = atoi( GetValue( line, "columns") );
    }
    else
    {
        tileSet->columns = 0;
    }
    if( GetValue( line, "source") != NULL )
    {
        strcpy( tileSet->source, GetValue( line, "source") );
    }
    if( GetValue( line, "sourceheight") != NULL )
    {
        tileSet->sourceHeight = atoi( GetValue( line, "sourceheight") );
    }
    else
    {
        tileSet->sourceHeight = 0;
    }
    if( GetValue( line, "sourcewidth") != NULL )
    {
        tileSet->sourceWidth = atoi( GetValue( line, "sourcewidth") );
    }
    else
    {
        tileSet->sourceWidth = 0;
    }
    if( GetValue( line, "tilesetid") != NULL )
    {
        tileSet->tileSetID = atoi( GetValue( line, "tilesetid") );
    }
    else
    {
        tileSet->tileSetID = 0;
    }
    if( GetValue( line, "tileoffsetx") != NULL )
    {
        tileSet->tileOffsetX = atoi( GetValue( line, "tileoffsetx") );
    }
    else
    {
        tileSet->tileOffsetX = 0;
    }
    if( GetValue( line, "tileoffsety") != NULL )
    {
        tileSet->tileOffsetY = atoi( GetValue( line, "tileoffsety") );
    }
    else
    {
        tileSet->tileOffsetY = 0;
    }
}

void PrintTMXTileSetValues( TMXTileSet tileSet )
{
    printf( "       firtGD          = %i \n", tileSet.firstGID      );
    printf( "       name            = %s \n", tileSet.name          );
    printf( "       tileWidth       = %i \n", tileSet.tileHeight        );
    printf( "       tileHeight      = %i \n", tileSet.tileWidth         );
    printf( "       tileCount       = %i \n", tileSet.tileCount         );
    printf( "       columns         = %i \n", tileSet.columns       );
    printf( "       source          = %s \n", tileSet.source            );
    printf( "       sourceWidth     = %i \n", tileSet.sourceWidth   );
    printf( "       sourceHeight    = %i \n", tileSet.sourceHeight  );
}

void PopulateTMXObjectGroup( XMLLine line, TMXObjectGroup* objectGroup )
{
    if( GetValue( line, "id") != NULL )
    {
        objectGroup->id = atoi( GetValue( line, "id") );
    }
    if( GetValue( line, "name") != NULL )
    {
        strcpy( objectGroup->name, GetValue( line, "name") );
    }
    if( GetValue( line, "color") != NULL )
    {
        strcpy( objectGroup->color, GetValue( line, "color") );
    }
    if( GetValue( line, "opacity") != NULL )
    {
        objectGroup->opacity = atof( GetValue( line, "opacity") );
    }
    else
    {
        objectGroup->opacity = 1.0;
    }
    if( GetValue( line, "visible") != NULL )
    {
        objectGroup->visible = atoi( GetValue( line, "visible") );
    }
    else
    {
        objectGroup->visible = 1;
    }
    if( GetValue( line, "locked") != NULL )
    {
        objectGroup->locked = atoi( GetValue( line, "locked") );
    }
    else
    {
        objectGroup->locked = 0;
    }
    if( GetValue( line, "offsetx") != NULL )
    {
        objectGroup->offsetX = atoi( GetValue( line, "offsetx") );
    }
    else
    {
        objectGroup->offsetX = 0;
    }
    if( GetValue( line, "offsety") != NULL )
    {
        objectGroup->offsetY = atoi( GetValue( line, "offsety") );
    }
    else
    {
        objectGroup->offsetY = 0;
    }
    if( GetValue( line, "draworder") != NULL )
    {
        strcpy( objectGroup->drawOrder, GetValue( line, "draworder") );
    }
}

void PrintTMXObjectGroupValues( TMXObjectGroup objectGroup )
{
    printf( "       id          = %i \n", objectGroup.id        );
    printf( "       name        = %s \n", objectGroup.name  );
    printf( "       visible     = %i \n", objectGroup.visible );
    printf( "       locked      = %i \n", objectGroup.locked    );
    printf( "       offsetX     = %i \n", objectGroup.offsetX );
    printf( "       offsetY     = %i \n", objectGroup.offsetY );
}

void PopulateTMXObject( XMLLine line, TMXObject* object )
{
    if( GetValue( line, "id") != NULL )
    {
        object->id = atoi( GetValue( line, "id") );
    }
    if( GetValue( line, "name") != NULL )
    {
        strcpy( object->name, GetValue( line, "name") );
    }
    if( GetValue( line, "x") != NULL )
    {
        object->pos.x = atoi( GetValue( line, "x") );
    }
    else
    {
        object->pos.x = 0;
    }
    if( GetValue( line, "y") != NULL )
    {
        object->pos.y = atoi( GetValue( line, "y") );
    }
    else
    {
        object->pos.y = 0;
    }
    if( GetValue( line, "width") != NULL )
    {
        object->width = atoi( GetValue( line, "width") );
    }
    else
    {
        object->width = 0;
    }
    if( GetValue( line, "height") != NULL )
    {
        object->height = atoi( GetValue( line, "height") );
    }
    else
    {
        object->height = 0;
    }
    if( GetValue( line, "type") != NULL )
    {
        strcpy( object->type, GetValue( line, "type") );
    }
    if( GetValue( line, "rotation") != NULL )
    {
        object->rotation = atof( GetValue( line, "rotation") );
    }
    else
    {
        object->rotation = 0;
    }
    if( GetValue( line, "gid") != NULL )
    {
        object->gid = atoi( GetValue( line, "gid") );
    }
    else
    {
        object->gid = 0;
    }
    if( GetValue( line, "visible") != NULL )
    {
        object->visible = atoi( GetValue( line, "visible") );
    }
    else
    {
        object->visible = 1;
    }
    if( GetValue( line, "typeid") != NULL )
    {
        object->typeID = atoi( GetValue( line, "typeid") );
    }
    else
    {
        object->typeID = 0;
    }
}

void PrintTMXObjectValues( TMXObject object )
{
    printf( "       id          = %i \n", object.id         );
    printf( "       X           = %f \n", object.pos.x  );
    printf( "       Y           = %f \n", object.pos.y  );
    printf( "       name        = %s \n", object.name   );
    printf( "       visible     = %i \n", object.visible    );
    printf( "       width       = %i \n", object.width  );
    printf( "       height      = %i \n", object.height     );
}

void PopulateTMXProperty( XMLLine line, TMXProperty* property )
{
    if( GetValue( line, "name") != NULL )
    {
        strcpy( property->name, GetValue( line, "name") );
    }
    if( GetValue( line, "type") != NULL )
    {
        strcpy( property->type, GetValue( line, "type") );
    }
    if( GetValue( line, "value") != NULL )
    {
        strcpy( property->value, GetValue( line, "value") );
    }


    property->intValue = 0;
    property->floatValue = 0.0;
    property->boolValue = 0;
    //property->fileValue = "default";
    //property->stringValue = "default";


    if( strcmp( "int", property->type ) == 0 )
    {
        property->intValue = atoi( property->value );
    }
    if( strcmp( "float", property->type ) == 0 )
    {
        property->floatValue = atof( property->value );
    }
    if( strcmp( "bool", property->type ) == 0 )
    {
        property->boolValue = atoi( property->value );
    }
    if( strcmp( "file", property->type ) == 0 )
    {
        strcpy( property->fileValue, property->value );
    }
    if( strcmp( "string", property->type ) == 0 )
    {
        strcpy( property->stringValue, property->value );
    }
}

void PrintTMXPropertyValues( TMXProperty property )
{
    printf( "           name    = %s \n", property.name            );
    printf( "           type    = %s \n", property.type            );
    printf( "           value   = %s \n", property.value           );
}

class TMXConverter
{
protected:
public:
    TMXConverter()
    {
        //filter();
        //newterm( NULL, NULL, NULL );
    }

    //==================== Level Methods =======================
    TMXMap LoadTMXMap( const char* filePath )
    {
        /*  The Method loads a tmx file in xml format (no json file etc).
            It does not do any error checking. Program will crash if File is inconsistent
            It can only handle square grids. no hex or isometric stuff
            it doesnt do text or polyline objects
            it cant handle groups (restructuring in subfunctions would be neccesary)
            lots of stuff is untested, like files withoud embedded tileset data or empty tilesets
            Its rather slow too...
            Use with caution 
        */


        TMXMap newTMXMap;


        //open File, read only (txt mode)
        FILE* file = fopen( filePath, "r" );
        if( file == NULL )
        {
            Quit( "Error opening TMX File" );
        }

        //read xml header
        XMLLine line = ExtractXMLLine( file );
        PrintXMLLine( line );
        getch();

        //read map tag
        line = ExtractXMLLine( file );
        PrintXMLLine( line );

        PopulateTMXMap( line, &newTMXMap );
        PrintTMXMapValues( newTMXMap );

        while( strcmp( "map", line.tag ) != 0 || !line.closing )
        {
            line = ExtractXMLLine( file );
            PrintXMLLine( line );

            if( strcmp( "tileset", line.tag ) == 0)
            {
                TMXTileSet tileSet;
                PopulateTMXTileSet( line, &tileSet );
                PrintTMXTileSetValues( tileSet );

                while( strcmp( "tileset", line.tag ) != 0 || !line.closing )
                {
                    line = ExtractXMLLine( file );
                    PrintXMLLine( line );

                    if( strcmp( "properties", line.tag ) == 0 )
                    {
                        while( strcmp( "properties", line.tag ) != 0 || !line.closing )
                        {
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );

                            if( strcmp( "property", line.tag ) == 0 )
                            {
                                TMXProperty property;
                                PopulateTMXProperty( line, &property );
                                PrintTMXPropertyValues( property );
                                while( strcmp( "property", line.tag ) != 0 || !line.closing )
                                {
                                    line = ExtractXMLLine( file );
                                    PrintXMLLine( line );
                                }

                                tileSet.properties.push_back( property );
                            }
                        }
                    }
                }

                newTMXMap.tileSets.push_back( tileSet );
            }

            if( strcmp( "layer", line.tag ) == 0)
            {
                TMXLayer layer;
                PopulateTMXLayer( line, &layer );
                PrintTMXLayerValues( layer );
                

                while( strcmp( "layer", line.tag ) != 0 || !line.closing )
                {
                    line = ExtractXMLLine( file );
                    PrintXMLLine( line );
                    if( strcmp( "data", line.tag ) == 0 )
                    {
                        if( strcmp( "base64", GetValue( line, "encoding") ) == 0 ) 
                        {
                            layer.data = (uint32_t*)malloc( layer.width * layer.height * sizeof(uint32_t) );
                            fread( layer.data, 1, layer.width * layer.height * sizeof(uint32_t), file );
                            //fscanf( file, "\n" );
                            
                            for( unsigned int i = 0; i < layer.width * layer.height; i++ )
                            {
                                printf("%u,", layer.data[i] );
                            }

                            printf( "\n" );
                        }

                        if( strcmp( "csv", GetValue( line, "encoding") ) == 0 ) 
                        {
                            layer.data = (uint32_t*)malloc( layer.width * layer.height * sizeof(uint32_t) );
                            
                            for( unsigned int y = 0; y < layer.height; y++ )
                            {
                                for( unsigned int x = 0; x < layer.width; x++ )
                                {
                                    if( y == layer.height - 1 && x == layer.width - 1 )
                                    {
                                        fscanf( file, "%u", &layer.data[y * layer.width + x] );
                                    }
                                    else
                                    {
                                        fscanf( file, "%u,", &layer.data[y * layer.width + x] );
                                    }
                                    printf( "%i,", layer.data[y * layer.width + x] );
                                }
                                fscanf( file, "\n" );
                                printf("\n");
                            }
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );
                            printf("test");
                        }
                    }

                    if( strcmp( "properties", line.tag ) == 0 )
                    {
                        while( strcmp( "properties", line.tag ) != 0 || !line.closing )
                        {
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );

                            if( strcmp( "property", line.tag ) == 0 )
                            {
                                TMXProperty property;
                                PopulateTMXProperty( line, &property );
                                PrintTMXPropertyValues( property );
                                while( strcmp( "property", line.tag ) != 0 || !line.closing )
                                {
                                    line = ExtractXMLLine( file );
                                    PrintXMLLine( line );
                                }

                                layer.properties.push_back( property );
                            }
                        }
                    }
                }

                newTMXMap.layers.push_back( layer );
            } 

            if( strcmp( "objectgroup", line.tag ) == 0 )
            {
                TMXObjectGroup objectGroup;
                PopulateTMXObjectGroup( line, &objectGroup );
                PrintTMXObjectGroupValues( objectGroup );

                while( strcmp( "objectgroup", line.tag ) != 0 || !line.closing )
                {
                    line = ExtractXMLLine( file );
                    PrintXMLLine( line );

                    if( strcmp( "object", line.tag ) == 0 )
                    {
                        TMXObject object;
                        PopulateTMXObject( line, &object );
                        PrintTMXObjectValues( object );

                        while( strcmp( "object", line.tag ) != 0 || !line.closing )
                        {
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );

                            if( strcmp( "properties", line.tag ) == 0 )
                            {
                                while( strcmp( "properties", line.tag ) != 0 || !line.closing )
                                {
                                    line = ExtractXMLLine( file );
                                    PrintXMLLine( line );

                                    if( strcmp( "property", line.tag ) == 0 )
                                    {
                                        TMXProperty property;
                                        PopulateTMXProperty( line, &property );
                                        PrintTMXPropertyValues( property );
                                        while( strcmp( "property", line.tag ) != 0 || !line.closing )
                                        {
                                            line = ExtractXMLLine( file );
                                            PrintXMLLine( line );
                                        }

                                        object.properties.push_back( property );
                                    }
                                }
                            }
                        }

                        objectGroup.objects.push_back( object );
                    }

                    if( strcmp( "properties", line.tag ) == 0 )
                    {
                        while( strcmp( "properties", line.tag ) != 0 || !line.closing )
                        {
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );

                            if( strcmp( "property", line.tag ) == 0 )
                            {
                                TMXProperty property;
                                PopulateTMXProperty( line, &property );
                                PrintTMXPropertyValues( property );
                                while( strcmp( "property", line.tag ) != 0 || !line.closing )
                                {
                                    line = ExtractXMLLine( file );
                                    PrintXMLLine( line );
                                }

                                objectGroup.properties.push_back( property );
                            }
                        }
                    }
                }

                newTMXMap.objectGroups.push_back( objectGroup );
            }

            if( strcmp( "properties", line.tag ) == 0 )
            {
                while( strcmp( "properties", line.tag ) != 0 || !line.closing )
                {
                    line = ExtractXMLLine( file );
                    PrintXMLLine( line );

                    if( strcmp( "property", line.tag ) == 0 )
                    {
                        TMXProperty property;
                        PopulateTMXProperty( line, &property );
                        PrintTMXPropertyValues( property );
                        while( strcmp( "property", line.tag ) != 0 || !line.closing )
                        {
                            line = ExtractXMLLine( file );
                            PrintXMLLine( line );
                        }

                        newTMXMap.properties.push_back( property );
                    }
                }
            }
        }

        fclose( file );

        return newTMXMap;
    }
    void WriteNMAP( const char* filePath, TMXMap map )
    {
        //open File, read only (txt mode)
        FILE* file = fopen( filePath, "wb+" );
        if( file == NULL )
        {
            Quit( "Error opening TMX File" );
        }

        //write magic
        if( writeSignature )
        {
	        char magic[3];
	        magic[0] = 'M';
	        magic[1] = 'A';
	        magic[2] = 'P';
	        fwrite( magic, 1, 3, file );
	    }

        //write map info
        uint32_t numLayers = map.layers.size();
        if( writeMapInfo )
        {
	        fwrite( &map.width, 1, sizeof( uint32_t ), file );
	        fwrite( &map.height, 1, sizeof( uint32_t ), file );
	        fwrite( &map.tileWidth, 1, sizeof( uint32_t ), file );
	        fwrite( &map.tileHeight, 1, sizeof( uint32_t ), file );
	        
	        fwrite( &numLayers, 1, sizeof( uint32_t ), file );
	    }

        //write layers
        if( numLayersToWrite == 0 )
        {
        	numLayersToWrite = 999;
        }
        for( unsigned int i = 0; i < numLayers && i < numLayersToWrite; i ++ )
        {
            //find highest guid
            unsigned int highestGUID = 0;
            for( unsigned int u = 0; u < map.layers[i].width * map.layers[i].height; u ++ )
            {
                if( map.layers[i].data[u] > highestGUID )
                {
                    highestGUID = map.layers[i].data[u];
                }
            }
            printf( "highestGUID=%i\n", highestGUID );

            //find fitting GUID;
            int guid = 0;
            int low = 1;
            for( unsigned int guids = 0; guids < map.tileSets.size(); guids++ )
            {
                if( highestGUID >= low && highestGUID <= map.tileSets[guids].firstGID )
                {
                    guid = map.tileSets[guids].firstGID;
                    break;
                }
                low = map.tileSets[guids].firstGID;
            }

            int tileSetID = 0;
            for( unsigned int properties = 0; properties < map.layers[i].properties.size(); properties++ )
            {
                if( strcmp( "tileSetID", map.layers[i].properties[properties].name ) == 0 )
                {
                    tileSetID = map.layers[i].properties[properties].intValue;
                    break;
                }
            }

            printf( "GUID=%i\n", guid );
            printf( "tileSetID=%i\n", tileSetID );

            if( writeLayerInfo )
            {
	            fwrite( &tileSetID, 1, sizeof( uint32_t ), file );
	            fwrite( &map.layers[i].width, 1, sizeof( uint32_t ), file );
	            fwrite( &map.layers[i].height, 1, sizeof( uint32_t ), file );
	            fwrite( &map.layers[i].offsetX, 1, sizeof( uint32_t ), file );
	            fwrite( &map.layers[i].offsetY, 1, sizeof( uint32_t ), file );
	        }
            for( unsigned int u = 0; u < map.layers[i].width * map.layers[i].height; u ++ )
            {
                char data = map.layers[i].data[u] - guid;
                fwrite( &data, 1, sizeof(uint8_t), file );
                if( writeMetaByte )
                {
                    fwrite( &metaByte, 1, sizeof(uint8_t), file );
                }
            }
        }

        //write object layers
        if( writeObjects )
        {
	        uint32_t numObjectLayers = map.objectGroups.size();
	        fwrite( &numObjectLayers, 1, sizeof( uint32_t ), file );
	        for( unsigned int z = 0; z < numObjectLayers; z ++ )
	        {
	            uint32_t numObjects = map.objectGroups[z].objects.size();
	            fwrite( &numObjects, 1, sizeof( uint32_t ), file );
	            for( unsigned int u = 0; u < numObjects; u ++ )
	            {
	                fwrite( &map.objectGroups[z].objects[u].id, 1, sizeof(uint32_t), file );
	                fwrite( &map.objectGroups[z].objects[u].pos.x, 1, sizeof(float), file );
	                fwrite( &map.objectGroups[z].objects[u].pos.y, 1, sizeof(float), file );
	            }
	        }
	    }

        fclose( file );
    }
    TMXProperty GetProperty( vector<TMXProperty> properties, const char* name )
    {
        TMXProperty dummy;
        dummy.intValue = 0;
        strcpy(dummy.stringValue, "");
        dummy.boolValue = false;

        for( unsigned int i = 0; i < properties.size(); i ++ )
        {
            if( strcmp( properties[i].name, name ) == 0 )
            {
                return properties[i];
            }
        }

        return dummy;
    }
    int GetFirstGid( TMXMap* in, int tileSetID )
    {
        return in->tileSets[tileSetID].firstGID;
        //
    }
    int GetTypeID( TMXMap* in, int mapValue, int TMXTileSetIndex )
    {
        mapValue = mapValue - GetFirstGid( in, TMXTileSetIndex );

        for( unsigned int u = 0; u < in->tileSets[TMXTileSetIndex].tiles.size(); u++ )
        {
            if( in->tileSets[TMXTileSetIndex].tiles[u].id == mapValue )
            {
                return in->tileSets[TMXTileSetIndex].tiles[u].typeID;
            }
        }

        return -1;
    }
    int GetTMXTileSetIndex( TMXMap* in, int mapValue )
    {
        //gets the TileSetIndex in the TMX File

        //printf( " tilesets %i \n ", in->tileSets.size() );
        for(int i = (int)in->tileSets.size()-1; i >= 0; i-- )
        {
            //printf( "index= %i, gid=%i checking for=%i \n ", i, in->tileSets[i].firstGID, mapValue - in->tileSets[i].firstGID );
            //getch();
            if( mapValue - in->tileSets[i].firstGID >= 0 )
            {
                //printf( "found at %i checked for %i \n", in->tileSets[i].firstGID, mapValue - in->tileSets[i].firstGID  );
                return i;
            }
        }

        return -1;
    }
    int GetTileSetID( TMXMap* in, int TMXTileSetIndex )
    {
        return in->tileSets[TMXTileSetIndex].tileSetID;
        //
    }
    int GetTileID( TMXMap* in, int mapValue, int TMXTileSetIndex )
    {
        return mapValue - GetFirstGid(in, TMXTileSetIndex);
        //
    }
    //void CreateObjectsFromMap( TMXMap* in )
    //{
        /*unsigned int width, height;
        unsigned int tileWidth, tileHeight;
        vector<TMXTileSet> tileSets;
        vector<TMXLayer> layers;
        vector<TMXObjectGroup> objectGroups;*/

        /*for( unsigned int i = 0; i < in->layers.size(); i++ )
        {
            for( unsigned int y = 0; y < in->layers[i].height; y++ )
            {
                for( unsigned int x = 0; x < in->layers[i].width; x++ )
                {
                    int mapValue = in->layers[i].data[y * in->layers[i].width + x];
                    //printf( "mapValue = %i\n", mapValue );
                    if( mapValue > 0 )
                    {
                        GameObject* newObject = NULL;

                        
                        //int tileSetID     = GetTileSetID( in, mapValue );
                        int tileSetIndexInTMX   = GetTMXTileSetIndex( in, mapValue );
                        int tileSetID           = GetTileSetID( in, tileSetIndexInTMX );
                        int typeID              = GetTypeID( in, mapValue, tileSetIndexInTMX );
                        int tileID              = GetTileID( in, mapValue, tileSetIndexInTMX );
                        int tileHeight          = in->tileSets[tileSetIndexInTMX].tileHeight;
                        int tileWidth           = in->tileSets[tileSetIndexInTMX].tileWidth;
                        
                        Vector2D newPos;
                        newPos.x    = ( x * tileWidth ) + in->layers[i].offsetX;
                        newPos.y    = ( y * tileHeight ) + in->layers[i].offsetY;
                        //newPos        = newPos + offset;  //additional offset from paramteer list

                        //printf("O %i:%i:%i mapValue=%i tileSetTMX=%i TMXSource=%s tileSetID=%i tileID=%i typeID=%i pos=%f:%f\n", i, y, x, mapValue, tileSetIndexInTMX, in->tileSets[tileSetIndexInTMX].source, tileSetID, tileID, typeID, newPos.x, newPos.y);

                        if( typeID == TYPE_PLAYER )
                        {
                            newObject = new Player( this );
                        }
                        else if( typeID == TYPE_SOLID )
                        {
                            newObject = new Solid( this );
                        }
                        else if( typeID == TYPE_SOLID_TOP )
                        {
                            newObject = new SolidTop( this );
                        }
                        else if( typeID == TYPE_BACK_GROUND ) 
                        {
                            newObject = new BackGround( this );
                        }
                        else if( typeID == TYPE_BACK_GROUND_ANIMATION ) 
                        {
                            newObject = new BackGroundAnimation( this );
                        }
                        else if( typeID == TYPE_TRAP ) 
                        {
                            newObject = new Trap( this );
                        }
                        else if( typeID == TYPE_EXIT ) 
                        {
                            newObject = new Exit( this );
                        }
                        else if( typeID == TYPE_TREASURE ) 
                        {
                            newObject = new Treasure( this );
                        }
                        else if( typeID == TYPE_BATTERY ) 
                        {
                            newObject = new Battery( this );
                        }
                        else if( typeID == TYPE_SHIP_WREK ) 
                        {
                            newObject = new ShipWrek( this );
                        }
                        else if( typeID == TYPE_DOOR ) 
                        {
                            newObject = new Door( this );
                        }
                        else if( typeID == TYPE_KEY ) 
                        {
                            newObject = new Key( this );
                        }
                        else if( typeID == TYPE_SYNTH_STATION ) 
                        {
                            newObject = new SynthStation( this );
                        }
                        else
                        {
                            //printf("create!\n");
                            if(tileID != -1)
                            {
                                newObject = new GameObject( this );
                        
                            }
                        }
                        if( newObject != NULL )
                        {
                            //newObject->SetTypeID  ( typeID );
                            newObject->SetPos       ( newPos );
                            newObject->SetDimensions( tileWidth, tileHeight );
                            newObject->SetTileSetID ( tileSetID );
                            newObject->SetTileIndex ( tileID );
                            newObject->SetDrawOrder ( i );

                            AddObject( newObject );
                        }
                        //getch();
                    }
                }
            }
        }

        for( unsigned int i = 0; i < in->objectGroups.size(); i++ )
        {
            for( unsigned int u = 0; u < in->objectGroups[i].objects.size(); u++ )
            {
                TMXObject* object;
                object = &in->objectGroups[i].objects[u];

                if( object->typeID != -1 )
                {
                    GameObject* newObject = NULL;

                    Vector2D newPos = object->pos;
                    newPos.x    = newPos.x + in->layers[i].offsetX;
                    newPos.y    = newPos.y + in->layers[i].offsetY;
                    //newPos        = newPos + offset;  //additional offset from paramteer list
                            
                    if( object->typeID == TYPE_PLAYER ) //placeholder
                    {
                        //printf("create!\n");
                        newObject = new Player( this );
                        

                    }

                    if( object->typeID == TYPE_PLAYER_TOP_DOWN ) //placeholder
                    {
                        //printf("create!\n");
                        newObject = new PlayerTopDown( this );
                        

                    }

                    if( object->typeID == TYPE_CITY_OVERWORLD ) //placeholder
                    {
                        //printf("create!\n");
                        newObject = new CityOverWorld( this );
                        CityOverWorld* city;
                        city = ( CityOverWorld* )newObject;

                        city->SetLevelPath( GetProperty( object->properties, "levelPath" ).stringValue );
                        city->SetTileIndex( GetProperty( object->properties, "tileIndex" ).intValue );
                    }

                    if( object->typeID == TYPE_CITY_OVERWORLD1X1 ) //placeholder
                    {
                        //printf("create!\n");
                        newObject = new CityOverWorld1x1( this );
                        CityOverWorld1x1* city;
                        city = ( CityOverWorld1x1* )newObject;

                        city->SetLevelPath( GetProperty( object->properties, "levelPath" ).stringValue );
                        city->SetTileIndex( GetProperty( object->properties, "tileIndex" ).intValue );
                    }

                    if( newObject != NULL )
                    {
                        //newObject->SetTypeID  ( object->typeID );         //get from property
                        newObject->SetPos       ( newPos );                 //get from pos
                        

                        AddObject( newObject );
                    }
                }
            }
        }

        FindLevelBoundaries();
    }*/
    void Quit(const char* message)
    {

        //Print Message
        printf( "Error: %s \n", message );
        printf( "Terminating Program" );

        //exit Program
        exit(EXIT_FAILURE);
    }
    //==========================================================
};



int main( int argc, char* argv[] )
{
    TMXConverter tmxConverter;
    TMXMap tmxMap;

    char* fileName = NULL;
    

    

    for( int i = 0; i < argc; i++ )
    {
        if( strcmp( argv[i], "--filename" ) == 0)
        {
            if( i + 1 <= argc )
            {
                fileName = argv[i+1];
            }
        }
        if( strcmp(argv[i], "--help") == 0 || strcmp( argv[i], "-h" ) == 0 )
		{
			printf("--filename - set input file name \n");
			printf("--writeSignature - write file signature \n");
			printf("--writeMapInfo - write map info after signature \n");
			printf("--writeLayerInfo  - write info before any layer \n");
			printf("--numLayers   - set the max number of layer to write \n");
			printf("--writeObjects   - write Object Layers \n");
            printf("--metaByte   - write another byte after every Map byte in a Layer \n");
			return 0;
		}
        if( strcmp(argv[i], "--writeSignature") == 0 )
		{
			writeSignature = true;
		}
		if( strcmp(argv[i], "--writeMapInfo") == 0 )
		{
			writeMapInfo = true;
		}
		if( strcmp(argv[i], "--writeLayerInfo") == 0 )
		{
			writeLayerInfo = true;
		}
		if( strcmp(argv[i], "--writeObjects") == 0 )
		{
			writeObjects = true;
		}
		if( strcmp(argv[i], "--numLayers") == 0 )
		{
			numLayersToWrite = atoi(argv[i + 1]);
		}
        if( strcmp(argv[i], "--metabyte") == 0 )
        {
            metaByte = atoi(argv[i + 1]);
            writeMetaByte = true;
        }
    }

    tmxMap = tmxConverter.LoadTMXMap( fileName );

    printf("Filename: %s\n", fileName);
    char* withoutExtension = strtok(fileName, ".");
    printf("just filename: %s\n", withoutExtension);
    char oldName[100];
    strcpy(oldName, fileName);

    string name, number, extension;
    name = withoutExtension;
    extension = ".nmap";
    string outName = name + number + extension;
    printf("new filename: %s\n", outName.c_str() );
    //file = fopen( outName.c_str(), "wb+" );

    tmxConverter.WriteNMAP( outName.c_str(), tmxMap );

    

    return 0;
}
//
// Copyright (c) 2008-2014 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Resource.h"
#include "Ptr.h"
#include "Sprite2D.h"

namespace Urho3D
{

class Sprite2D;
class Texture2D;
class TmxFile2D;
class XMLElement;
class XMLFile;

/// Tmx peroperies.
struct URHO3D_API TmxProperties2D
{
    /// Return has property (use for script).
    bool HasProperty(const String& name) const;
    /// Return property value (use for script).
    const String& GetProperty(const String& name) const;

    /// Properties.
    HashMap<String, String> properties_;
};

/// Tmx layer type.
enum TmxLayerType2D
{
    /// Tile layer.
    LT_TILE_LAYER = 0,
    /// Object group.
    LT_OBJECT_GROUP,
    /// Image layer.
    LT_IMAGE_LAYER,
    /// Invalid.
    LT_INVALID,
};

/// Tmx layer.
struct URHO3D_API TmxLayer2D : TmxProperties2D
{
    TmxLayer2D(TmxFile2D* tmxFile, TmxLayerType2D type);
    virtual ~TmxLayer2D();

    /// Tmx file.
    WeakPtr<TmxFile2D> tmxFile_;
    /// Layer type.
    TmxLayerType2D type_;
    /// Name.
    String name_;
    /// Width.
    int width_;
    /// Height.
    int height_;
    /// Visible.
    bool visible_;
};

/// Tmx tile.
struct URHO3D_API TmxTile
{
    TmxTile();

    /// Return has property.
    bool HasProperty(const String& name) const;
    /// Return property.
    const String& GetProperty(const String& name) const;

    /// Gid.
    int gid_;
    /// Sprite.
    SharedPtr<Sprite2D> sprite_;
    /// Properties.
    const TmxProperties2D* properties_;
};

/// Tmx tile layer.
struct URHO3D_API TmxTileLayer2D : TmxLayer2D
{
    TmxTileLayer2D(TmxFile2D* tmxFile);

    /// Tile.
    Vector<TmxTile> tileGids_;
};

/// Object type.
enum TmxObjectType
{
    /// Rectangle.
    OT_RECTANGLE = 0,
    /// Ellipse.
    OT_ELLIPSE,
    /// Polygon.
    OT_POLYGON,
    /// Polyline.
    OT_POLYLINE,
    /// Tile.
    OT_TILE,
};

/// Tmx object.
struct URHO3D_API TmxObject2D : TmxProperties2D
{
    /// Return number of points (use for script).
    unsigned GetNumPoints() const;
    /// Return point at index (use for script).
    const Vector2& GetPoint(unsigned index) const;
    
    /// Object type.
    TmxObjectType type_;
    /// Position.
    float x_, y_;
    /// Size (for rectangle and ellipse).
    float width_, height_;
    /// Points(for polygon and polyline).
    Vector<Vector2> points_;

    /// Gid (for tile).
    int gid_;
    /// Sprite (for tile).
    SharedPtr<Sprite2D> sprite_;
};

/// Tmx image layer.
struct URHO3D_API TmxObjectGroup2D : TmxLayer2D
{
    TmxObjectGroup2D(TmxFile2D* tmxFile);

    /// Objects.
    Vector<TmxObject2D> objects_;
};

/// Tmx image layer.
struct URHO3D_API TmxImageLayer2D : TmxLayer2D
{
    TmxImageLayer2D(TmxFile2D* tmxFile);

    /// Sprite.
    SharedPtr<Sprite2D> sprite_;
};

/// Tile map file.
class URHO3D_API TmxFile2D : public Resource
{
    OBJECT(TmxFile2D);

public:
    /// Construct.
    TmxFile2D(Context* context);
    /// Destruct.
    virtual ~TmxFile2D();
    /// Register object factory. 
    static void RegisterObject(Context* context);

    /// Load resource from stream. May be called from a worker thread. Return true if successful.
    virtual bool BeginLoad(Deserializer& source);
    /// Finish resource loading. Always called from the main thread. Return true if successful.
    virtual bool EndLoad();

    /// Return width.
    int GetWidth() const { return width_; }
    /// Return height.
    int GetHeight() const { return height_; }
    /// Return tile width.
    float GetTileWidth() const { return tileWidth_; }
    /// Return tile height.
    float GetTileHeight() const { return tileHeight_; }
    /// Return number of layers.
    unsigned GetNumLayers() const { return layers_.Size(); }
    /// Return layer at index.
    const TmxLayer2D* GetLayer(unsigned index) const;
    /// Return layer by name.
    const TmxLayer2D* GetLayerByName(const String& name) const;

private:
   /// Load tile set.
    bool LoadTileSet(const XMLElement& element);
    /// Load layer.
    bool LoadLayer(const XMLElement& element);
    /// Return tile sprite by gid.
    Sprite2D* GetTileSprite(int gid) const;
    /// Return tile properties by gid.
    const TmxProperties2D* GetTileProperties(int gid) const;
    /// Load object group.
    bool LoadObjectGroup(const XMLElement& element);
    /// Load image layer.
    bool LoadImageLayer(const XMLElement& element);
    /// Load layer info.
    void LoadLayerInfo(const XMLElement& element, TmxLayer2D* layer);
    /// Load properties.
    void LoadProperties(const XMLElement& element, TmxProperties2D& peoperties);

    /// XML file used during loading.
    SharedPtr<XMLFile> loadXMLFile_;
    /// Width.
    int width_;
    /// Height.
    int height_;
    /// Tile width.
    float tileWidth_;
    /// Tile height.
    float tileHeight_;
    /// Tile set textures.
    Vector<SharedPtr<Texture2D> > tileSetTextures_;
    /// Gid to tile sprite mapping.
    HashMap<int, SharedPtr<Sprite2D> > tileSprites_;
    /// Gid to tile properties mapping.
    HashMap<int, TmxProperties2D> tileProperties_;
    /// Layers.
    Vector<TmxLayer2D*> layers_;
};

}


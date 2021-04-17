# Material Docs

A material defines a set of settings which represent how a mesh is shaded and colored. Below is the schema of a material. Most options are optional and have a default value, or simply do not apply if not set.

# Schema

```json
{
  "name": {
    "type": "string",
    "description": "user-friendly name for this instance of a material. Example: 'Robot' might refer to a material used to render a metallic robot",
    "required": true
  },
  "material_type": {
    "type": "string",
    "description": "type of material determine which passes are involved in rendering objects using this material",
    "required": true
  },
  "diffuse_texture": {
    "type": "object",
    "default": null,
    "required": false,
    "texture_uri": {
      "type": "string",
      "required" : true,
      "description": "resource name for the diffuse texture which should be loaded and then subsequently bound before drawing"
    },
    "sampler_settings": {
      "type": "object",
      "default": "<TODO>",
      "description": "Object containing optional sampler settings which should be applied when binding the texture",
      "texture_wrap_mode_u" : "string",
      "texture_wrap_mode_v" : "string",
      "texture_mag_filter": "string",
      "texture_min_filter": "string"
    }
  },
  "receives_lighting" : {
    "type": "boolean",
    "description": "Whether this object should be lit by external lighting",
    "default": true,
    "required": false
  },
  "uv_multiplier": {
    "type": "number",
    "default": "1.0",
    "required": false
  }
}
```
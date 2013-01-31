#include "../../debug.h"

#include "keyframemesh.h"
#include "keyframe.h"
#include "keyframemeshfile.h"
#include "keyframemeshtriangle.h"
#include "../../graphics/vertexbuffer.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"
#include "../../support/animationsequence.h"

#include <string.h>

KeyframeMesh::KeyframeMesh(const KeyframeMeshFile *file)
	: Content()
{
	STACK_TRACE;
	m_numFrames = file->GetNumFrames();
	m_frames = new Keyframe*[m_numFrames];
	ASSERT(m_frames != NULL);
	for (uint32_t i = 0; i < m_numFrames; ++i)
	{
		Keyframe *srcFrame = file->GetFrames()[i];
		uint32_t numVertices = srcFrame->GetNumVertices();

		Keyframe *frame = new Keyframe(srcFrame->GetName(), numVertices);
		ASSERT(frame != NULL);
		memcpy(frame->GetVertices(), srcFrame->GetVertices(), sizeof(Vector3) * numVertices);
		memcpy(frame->GetNormals(), srcFrame->GetNormals(), sizeof(Vector3) * numVertices);

		m_frames[i] = frame;
	}

	m_numTexCoords = file->GetNumTexCoords();
	m_texCoords = new Vector2[m_numTexCoords];
	ASSERT(m_texCoords != NULL);
	memcpy(m_texCoords, file->GetTexCoords(), sizeof(Vector2) * m_numTexCoords);

	m_numTriangles = file->GetNumTriangles();
	m_triangles = new KeyframeMeshTriangle[m_numTriangles];
	ASSERT(m_triangles != NULL);
	memcpy(m_triangles, file->GetTriangles(), sizeof(KeyframeMeshTriangle) * m_numTriangles);

	AnimationList::iterator itor;
	AnimationList srcAnimations = file->GetAnimations();
	for (itor = srcAnimations.begin(); itor != srcAnimations.end(); ++itor)
	{
		AnimationSequence sequence;
		sequence.start = itor->second.start;
		sequence.stop = itor->second.stop;
		m_animations[itor->first] = sequence;
	}

	m_numVerticesPerFrame = file->GetNumVerticesPerFrame();

	m_vertices = new VertexBuffer(BUFFEROBJECT_USAGE_STREAM);
	ASSERT(m_vertices != NULL);
	m_vertices->AddAttribute(ATTRIB_SIZE_VEC3);  // position for frame 1
	m_vertices->AddAttribute(ATTRIB_SIZE_VEC3);  // position for frame 2
	m_vertices->AddAttribute(ATTRIB_SIZE_VEC3);  // normal for frame 1
	m_vertices->AddAttribute(ATTRIB_SIZE_VEC3);  // normal for frame 2
	m_vertices->AddAttribute(VERTEX_TEXCOORD);
	m_vertices->Create(m_numVerticesPerFrame);
}

KeyframeMesh::~KeyframeMesh()
{
	STACK_TRACE;
	for (uint32_t i = 0; i < m_numFrames; ++i)
		SAFE_DELETE(m_frames[i]);
	SAFE_DELETE_ARRAY(m_frames);
	SAFE_DELETE_ARRAY(m_texCoords);
	SAFE_DELETE_ARRAY(m_triangles);
	SAFE_DELETE(m_vertices);
}

const AnimationSequence* KeyframeMesh::GetAnimation(const stl::string &name) const
{
	STACK_TRACE;
	AnimationList::const_iterator itor = m_animations.find(name);
	if (itor != m_animations.end())
		return &itor->second;
	else
		return NULL;
}
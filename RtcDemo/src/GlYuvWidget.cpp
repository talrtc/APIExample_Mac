#include "GlYuvWidget.h"
#include <QDebug>


#define VERTEXIN 0
#define TEXTUREIN 1

GLYuvWidget::GLYuvWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	yuvPtr = new uchar[3840 * 2160 * 3];
}

GLYuvWidget::~GLYuvWidget()
{
	makeCurrent();

	if (textureY) {
		textureY->destroy();
		delete textureY;
		textureY = nullptr;
	}

	if (textureU) {
		textureU->destroy();
		delete textureU;
		textureU = nullptr;
	}

	if (textureV) {
		textureV->destroy();
		delete textureV;
		textureV = nullptr;
	}

	if (program) {
		program->release();
		program->removeAllShaders();
		delete program;
		program = nullptr;
	}

	if (vshader) {
		delete vshader;
		vshader = nullptr;
	}

	if (fshader) {
		delete fshader;
		fshader = nullptr;
	}

	vbo.release();
	vbo.destroy();

	doneCurrent();

	delete[] yuvPtr;
}

void GLYuvWidget::showYuv(uchar *ptr, uint width, uint height)
{
	if (ptr) {
		if (yuvPtr) {
			memcpy(yuvPtr, ptr, width * height * 3 / 2);
		}

		//yuvPtr = ptr;
		videoW = width;
		videoH = height;
		update();
	}
}

void GLYuvWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	static const GLfloat vertices[]{
		// vetex coor
		-1.0f, -1.0f,
			-1.0f, +1.0f,
			+1.0f, +1.0f,
			+1.0f, -1.0f,
			// texture coor
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
	};


	vbo.create();
	vbo.bind();
	vbo.allocate(vertices, sizeof(vertices));

	vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
		"attribute vec4 vertexIn; \
	    attribute vec2 textureIn; \
	    varying vec2 textureOut;  \
	    void main(void)           \
	    {                         \
	        gl_Position = vertexIn; \
	        textureOut = textureIn; \
	    }";
	vshader->compileSourceCode(vsrc);

	fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc = "varying vec2 textureOut; \
				       uniform sampler2D tex_y; \
				       uniform sampler2D tex_u; \
				       uniform sampler2D tex_v; \
				       void main(void) \
				       { \
				           vec3 yuv; \
				           vec3 rgb; \
				           yuv.x = texture2D(tex_y, textureOut).r; \
				           yuv.y = texture2D(tex_u, textureOut).r - 0.5; \
				           yuv.z = texture2D(tex_v, textureOut).r - 0.5; \
				           rgb = mat3( 1,       1,         1, \
	                       0,       -0.39465,  2.03211, \
	                       1.13983, -0.58060,  0) * yuv; \
				           gl_FragColor = vec4(rgb, 1); \
				       }";
	fshader->compileSourceCode(fsrc);

	program = new QOpenGLShaderProgram(this);
	program->addShader(vshader);
	program->addShader(fshader);
	program->bindAttributeLocation("vertexIn", VERTEXIN);
	program->bindAttributeLocation("textureIn", TEXTUREIN);
	program->link();
	program->bind();
	program->enableAttributeArray(VERTEXIN);
	program->enableAttributeArray(TEXTUREIN);
	program->setAttributeBuffer(VERTEXIN, GL_FLOAT, 0, 2, 2 * sizeof(GLfloat));
	program->setAttributeBuffer(TEXTUREIN, GL_FLOAT, 8 * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));

	textureUniformY = program->uniformLocation("tex_y");
	textureUniformU = program->uniformLocation("tex_u");
	textureUniformV = program->uniformLocation("tex_v");
	textureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureV = new QOpenGLTexture(QOpenGLTexture::Target2D);
	textureY->create();
	textureU->create();
	textureV->create();
	idY = textureY->textureId();
	idU = textureU->textureId();
	idV = textureV->textureId();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//修改为1字节对齐，解决宽高不为4倍数时渲染图像异常问题。

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void GLYuvWidget::paintGL()
{
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, idY); 
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoW, videoH, 0, GL_RED, GL_UNSIGNED_BYTE, yuvPtr);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE1, idU);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoW >> 1, videoH >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, yuvPtr + videoW * videoH);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, idV);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoW >> 1, videoH >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, yuvPtr + videoW * videoH * 5 / 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glUniform1i(textureUniformY, 0);	
	glUniform1i(textureUniformU, 1);	
	glUniform1i(textureUniformV, 2);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

#ifndef GLYUVWIDGET_H
#define GLYUVWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>

// reference: https://blog.csdn.net/wanghualin033/article/details/79683836

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLYuvWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	GLYuvWidget(QWidget *parent = 0);
	~GLYuvWidget();


	void showYuv(uchar *ptr, uint width, uint height); // show a yuv frame

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	QOpenGLShaderProgram *program = nullptr;
	QOpenGLShader* vshader = nullptr, * fshader = nullptr;
	QOpenGLBuffer vbo;
	GLuint textureUniformY, textureUniformU, textureUniformV; // y/u/v position in opengl
	QOpenGLTexture *textureY = nullptr, *textureU = nullptr, *textureV = nullptr;
	GLuint idY, idU, idV; // texture object ID, return 0 when error
	uint videoW = 0, videoH = 0;
	uchar *yuvPtr = nullptr;
};

#endif
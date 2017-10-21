#include "Shader.h"

// Constructeurs et Destructeur

Shader::Shader() : vertexID(0), fragmentID(0), programID(0), vertexSource(), fragmentSource()
{
}


Shader::Shader(Shader const &shaderACopier)
{
    // Copie des fichiers sources

    vertexSource = shaderACopier.vertexSource;
    fragmentSource = shaderACopier.fragmentSource;


    // Chargement du nouveau shader

    charger();
}


Shader::Shader(std::string vertexSource, std::string fragmentSource):
    vertexID(0), fragmentID(0), programID(0),
    vertexSource(vertexSource), fragmentSource(fragmentSource)
{
    std::cout << this->vertexSource << " " << this->fragmentSource << std::endl;
}


Shader::~Shader()
{
    // Destruction du shader
    std::cout << "Destructor" << std::endl;
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    glDeleteProgram(programID);
}


// Methodes

Shader& Shader::operator=(Shader const &shaderACopier)
{
    // Copie des fichiers sources

    vertexSource = shaderACopier.vertexSource;
    fragmentSource = shaderACopier.fragmentSource;


    // Chargement du nouveau shader

    charger();


    // Retour du pointeur this

    return *this;
}


bool Shader::charger()
{
    // Destruction d'un �ventuel ancien Shader
    if(glIsShader(vertexID) == GL_TRUE)
        glDeleteShader(vertexID);
    
    if(glIsShader(fragmentID) == GL_TRUE)
        glDeleteShader(fragmentID);
    
    if(glIsProgram(programID) == GL_TRUE)
        glDeleteProgram(programID);

    // Compilation des shaders

    if(!compilerShader(vertexID, GL_VERTEX_SHADER, vertexSource))
        return false;

    if(!compilerShader(fragmentID, GL_FRAGMENT_SHADER, fragmentSource))
        return false;


    // Cr�ation du programme

    programID = glCreateProgram();


    // Association des shaders

    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);


    // Verrouillage des entr�es shader

    glBindAttribLocation(programID, 0, "in_Vertex");
    glBindAttribLocation(programID, 1, "in_Color");
    glBindAttribLocation(programID, 2, "in_TexCoord0");


    // Linkage du programme

    glLinkProgram(programID);


    // V�rification du linkage

    GLint erreurLink(0);
    glGetProgramiv(programID, GL_LINK_STATUS, &erreurLink);

    // S'il y a eu une erreur

    if(erreurLink != GL_TRUE)
    {
        // R�cup�ration de la taille de l'erreur

        GLint tailleErreur(0);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de m�moire

        char *erreur = new char[tailleErreur + 1];


        // R�cup�ration de l'erreur

        glGetShaderInfoLog(programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur

        std::cout << erreur << std::endl;


        // Lib�ration de la m�moire et retour du bool�en false

        delete[] erreur;
        glDeleteProgram(programID);

        return false;
    }

    // Sinon c'est que tout s'est bien pass�

    else
        return true;
}


bool Shader::compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource)
{
    // Cr�ation du shader

    shader = glCreateShader(type);

    // V�rification du shader

    if(shader == 0)
    {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }


    // Flux de lecture

    std::ifstream fichier(fichierSource.c_str());


    // Test d'ouverture

    if(!fichier)
    {
        std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
        glDeleteShader(shader);

        return false;
    }


    // Strings permettant de lire le code source

    std::string ligne;
    std::string codeSource;


    // Lecture

    while(getline(fichier, ligne))
        codeSource += ligne + '\n';

    // Fermeture du fichier

    fichier.close();

    // R�cup�ration de la chaine C du code source

    const GLchar* chaineCodeSource = codeSource.c_str();


    // Envoi du code source au shader

    glShaderSource(shader, 1, &chaineCodeSource, 0);


    // Compilation du shader

    glCompileShader(shader);


    // V�rification de la compilation

    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);


    // S'il y a eu une erreur

    if(erreurCompilation != GL_TRUE)
    {
        // R�cup�ration de la taille de l'erreur

        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de m�moire

        char *erreur = new char[tailleErreur + 1];


        // R�cup�ration de l'erreur

        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur

        std::cout << erreur << std::endl;


        // Lib�ration de la m�moire et retour du bool�en false

        delete[] erreur;
        glDeleteShader(shader);

        return false;
    }


    // Sinon c'est que tout s'est bien pass�

    else
        return true;
}


// Getter

GLuint Shader::getProgramID() const
{
    return programID;
}

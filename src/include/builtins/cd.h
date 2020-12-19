#ifndef CD_H_
#define CD_H_

/**
 * @brief Changes the directory if the given path is correct. Returns -1 if
 * impossible, 0 otherwise.
 *
 * Options: (not implement yet)
 *  -@	on systems that support it, present a file with extended
 *      attributes as a directory containing the file attributes
 *
 * @param argv
 * @return int
 */
int b_cd(char **argv);

/**
 * @brief Updates the actual path
 *
 * @param path
 * @param section
 * @return char*
 */
char *update_path(char *path, char *section);

#endif /* CD_H_ */
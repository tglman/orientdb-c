#ifndef O_CLASS_H_
#define O_CLASS_H_
#include "o_property.h"

struct o_class;

/*! \brief Retrieve the name of class from class
 *
 * \param class where retrieve name.
 * \return the name of class.
 */
char * o_class_get_name(struct o_class * class);

/*! \brief Retrieve the class id from class.
 *
 * \param class where retrieve id.
 * \return id of class.
 */
int o_class_get_id(struct o_class * class);

/*! \brief Set the id of class.
 *
 * \param class where set id.
 * \param id to set.
 */
void o_class_set_id(struct o_class * class, int id);

/*! \brief Retrieve the superclass of class.
 *
 * \param class where retrieve the superclass.
 * \return the superclass or 0 if not exist superclass.
 */
struct o_class * o_class_get_superclass(struct o_class * class);

/*! \brief Set the superclass of class.
 *
 * \param class to set superclass.
 * \param superclass to set.
 */
void o_class_set_superclass(struct o_class * class, struct o_class * superclass);

/*! \brief Retrieve a property from class.
 *
 * \param class where retrieve property.
 * \param name of property to retrieve.
 * \return the property or 0 if the property not exist.
 */
struct o_property * o_class_get_property(struct o_class * class, char * property_name);

/*! \brief Create a new property on class with specialized type.
 *
 * \param class where create.
 * \param name of property to create.
 * \param type of property to create.
 * \return the new property.
 */
struct o_property * o_class_create_property(struct o_class * class, char * property_name, enum o_document_value_type type);

/*! \brief Retrieve the array of class properties.
 *
 * \param class where retrieve properties.
 * \param n_properties filled with the number of properties.
 * \return the array of properties.
 */
struct o_property ** o_class_properties(struct o_class * class, int * n_properties);

/*! \brief Retrieve the id of default class cluster.
 *
 * \param class where retrieve id.
 * \return the id of class.
 */
int o_class_get_default_cluster(struct o_class * class);

/*! \brief Set the id of default class cluster.
 *
 * \param class where set  id.
 * \param cluster_id the id to set.
 */
void o_class_set_default_cluster(struct o_class * class, int cluster_id);

/*! \brief retrieve the array of cluster where was saved the class.
 *
 * \param class where retrieve.
 * \param n_cluster filled with the size of returned array.
 * \return the array that contain all id of cluster.
 */
const int * o_class_get_clusters(struct o_class * class, int * n_cluster);

/*! \brief Add a cluster id to the class.
 *
 * \param class where add the cluster.
 * \param clusterId to add.
 */
void o_class_add_cluster(struct o_class * class, int clusterId);

/*! \brief Remove a cluster id to the class.
 *
 * \param class where remove the cluster.
 * \param clusterId to remove.
 */
void o_class_remove_cluster(struct o_class * class, int clusterId);

/*! \brief Retrieve if the property with property_name exist.
 *
 * \param class where check.
 * \param property_name to check.
 * \return 1 if property exist otherwise 0.
 */
int o_class_exist_property(struct o_class * class, char * property_name);

/*! \brief Remove the property from the name.
 *
 * \param class where remove property.
 * \param property_name the name of property to remove.
 *
 */
void o_class_remove_property(struct o_class * class, char * property_name);

#endif /* O_CLASS_H_ */

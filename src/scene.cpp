/*
** scene.cpp
**
** This file is part of mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scene.h"
#include "globalstate.h"

#include <QDebug>

Scene::Scene()
{
	geometry.xOrigin = geometry.yOrigin = 0;
	geometry.rect = IntRect();
}

void Scene::insert(SceneElement &element)
{
	IntruListLink<SceneElement> *iter;

	for (iter = elements.begin(); iter != elements.end(); iter = iter->next)
	{
		SceneElement *e = iter->data;

		if (element.z <= e->z)
		{
			if (element.z == e->z)
				if (element.creationStamp > e->creationStamp)
					continue;

			elements.insertBefore(element.link, *iter);
			return;
		}
	}

	elements.append(element.link);
}

void Scene::reinsert(SceneElement &element)
{
	elements.remove(element.link);
	insert(element);
}

void Scene::notifyGeometryChange()
{
	IntruListLink<SceneElement> *iter;

	for (iter = elements.begin(); iter != elements.end(); iter = iter->next)
	{
		iter->data->onGeometryChange(geometry);
	}
}

void Scene::composite()
{
	IntruListLink<SceneElement> *iter;

	for (iter = elements.begin(); iter != elements.end(); iter = iter->next)
	{
		SceneElement *e = iter->data;

		if (e->visible)
			e->draw();
	}
}


SceneElement::SceneElement(Scene &scene, int z)
    : link(this),
      creationStamp(gState->genTimeStamp()),
      z(z),
      visible(true),
      scene(&scene)
{
	scene.insert(*this);
}

SceneElement::~SceneElement()
{
	unlink();
}

void SceneElement::setScene(Scene &scene)
{
	unlink();

	this->scene = &scene;

	scene.insert(*this);

	onGeometryChange(scene.getGeometry());
}

int SceneElement::getZ() const
{
	aboutToAccess();

	return z;
}

void SceneElement::setZ(int value)
{
	aboutToAccess();

	if (z == value)
		return;

	z = value;
	scene->reinsert(*this);
}

bool SceneElement::getVisible() const
{
	aboutToAccess();

	return visible;
}

void SceneElement::setVisible(bool value)
{
	aboutToAccess();

	visible = value;
}

void SceneElement::unlink()
{
	scene->elements.remove(link);
}

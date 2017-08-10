/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Zeeshan Ali <zeeshan.ali@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "blueplayer.h"
#include "iostream"

BluePlayer::BluePlayer(QObject *parent) : QObject(parent)
{
}

BluePlayer::BluePlayer(QDBusObjectPath playerPath, QObject *parent) : QObject(parent)
{
    m_mediaPlayer = new QDBusInterface ("org.bluez",
                                      playerPath.path(),
                                      "org.bluez.MediaPlayer1",
                                      QDBusConnection::systemBus());

    qDebug() << "playerPath is: " << playerPath.path();
    if (!m_mediaPlayer->connection().connect("org.bluez",
                                           playerPath.path(),
                                           "org.freedesktop.DBus.Properties",
                                           "PropertiesChanged",
                                           this,
                                           SLOT(onPropertiesChanged(const QString, const QMap<QString,QVariant>, const QStringList)))) {
        qWarning() << "Failed to connect PropertiesChanged signal";
    }
}

BluePlayer::~BluePlayer()
{
}

QString BluePlayer::getCachedStringProperty(const QString &name)
{
    if (m_properties.contains(name)) {
        return m_properties[name].toString();
    }

    m_properties[name] = m_mediaPlayer->property(name.toStdString().c_str()).toString();
    return m_properties[name].toString();
}

int BluePlayer::getCachedIntProperty(const QString &name)
{
    if (m_properties.contains(name)) {
        return m_properties[name].toInt();
    }

    m_properties[name] = m_mediaPlayer->property(name.toStdString().c_str()).toInt();
    return m_properties[name].toInt();
}

QMap<QString, QVariant> BluePlayer::getCachedMapProperty(const QString &name)
{
    if (m_properties.contains(name)) {
        return m_properties[name].toMap();
    } else {
        /* Fetching the property causes a SEGFAULT .. So.. Don't.*/
        return QMap<QString, QVariant>();
    }
}

QString BluePlayer::getCachedMapPropertyString(const QString &name, const QString &property)
{
    QMap<QString, QVariant> trackMap = getCachedMapProperty(name);
    if (trackMap.contains(property)) {
        return trackMap[property].toString();
    } else {
        return "Unknown";
    }
}

int BluePlayer::getCachedMapPropertyInt(const QString &name, const QString &property)
{
    QMap<QString, QVariant> trackMap = getCachedMapProperty(name);
    if (trackMap.contains(property)) {
        return trackMap[property].toInt();
    } else {
        return 0;
    }
}

void BluePlayer::play()
{
    std::cout << "Now playing" << std::endl;

    m_mediaPlayer->call("Play");
}

void BluePlayer::pause()
{
    std::cout << "Now paused" << std::endl;

    m_mediaPlayer->call("Pause");
}

void BluePlayer::stop()
{
    std::cout << "Now stopped" << std::endl;

    m_mediaPlayer->call("Stop");
}

void BluePlayer::next()
{
    std::cout << "Next" << std::endl;

    m_mediaPlayer->call("Next");
}

void BluePlayer::prev()
{
    std::cout << "Previous" << std::endl;

    m_mediaPlayer->call("Previous");
}

QString BluePlayer::getTitle()
{
    return getCachedMapPropertyString("Track", "Title");
}

QString BluePlayer::getStatus()
{
    return getCachedStringProperty("Status");
}

int BluePlayer::getDuration() {
    return getCachedMapPropertyInt("Track", "Duration");
}

QString BluePlayer::getAlbum() {
    return getCachedMapPropertyString("Track", "Album");
}

int BluePlayer::getTrackNumber() {
    return getCachedMapPropertyInt("Track", "TrackNumber");
}

QString BluePlayer::getArtist() {
    return getCachedMapPropertyString("Track", "Artist");
}

int BluePlayer::getNumberOfTracks() {
    return getCachedMapPropertyInt("Track", "NumberOfTracks");
}

QString BluePlayer::getGenre() {
    return getCachedMapPropertyString("Track", "Genre");
}

QString BluePlayer::getShuffle() {
    return getCachedStringProperty("Shuffle");
}

QString BluePlayer::getRepeat() {
    return getCachedStringProperty("Repeat");
}

int BluePlayer::getPosition() {
    /* This property is not updated automatically */
    return m_mediaPlayer->property("Position").toInt();
}

void BluePlayer::onPropertiesChanged(const QString path,
                                     const QMap<QString,QVariant> changedProperties,
                                     const QStringList invalidatedProperties)
{
    QMapIterator<QString, QVariant> i(changedProperties);
    while (i.hasNext()) {
        i.next();
        m_properties[i.key()] = i.value();

        if (i.key() == "Status") {
            emit statusChanged(getStatus());
        } else if (i.key() == "Position") {
            emit positionChanged(getPosition());
        } else if (i.key() == "Repeat") {
            emit repeatChanged(getRepeat());
        } else if (i.key() == "Shuffle") {
            emit shuffleChanged(getShuffle());
        } else if (i.key() == "Track") {
            /* Do special handling for this property */
            m_properties[i.key()] = qdbus_cast<QMap<QString, QVariant> >(i.value());

            /* Assume all properties changed */
            emit titleChanged(getTitle());
            emit durationChanged(getDuration());
            emit albumChanged(getAlbum());
            emit trackNumberChanged(getTrackNumber());
            emit artistChanged(getArtist());
            emit numberOfTracksChanged(getNumberOfTracks());
            emit genreChanged(getGenre());
        } else {
            qDebug() << "Unhandled property: " << i.key() << "[" << i.value() << "]";
        }
    }
}

/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Zeeshan Ali <zeeshan.ali@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BLUEPLAYER_H
#define BLUEPLAYER_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtDBus/QtDBus>

class BluePlayer : public QObject
{
    Q_OBJECT

public:
    BluePlayer(QObject *parent=0); // Only need this constructor for satisfying QML
    BluePlayer(QDBusObjectPath playerPath, QObject *parent=0);
    ~BluePlayer();

    /* Metadata properties */
    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
    Q_PROPERTY(int duration READ getDuration NOTIFY durationChanged)
    Q_PROPERTY(QString album READ getAlbum NOTIFY albumChanged)
    Q_PROPERTY(int trackNumber READ getTrackNumber NOTIFY trackNumberChanged)
    Q_PROPERTY(QString artist READ getArtist NOTIFY artistChanged)
    Q_PROPERTY(int numberOfTracks READ getNumberOfTracks NOTIFY numberOfTracksChanged)
    Q_PROPERTY(QString genre READ getGenre NOTIFY genreChanged)

    Q_PROPERTY(QString status READ getStatus NOTIFY statusChanged)

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();

signals:
    void titleChanged(QString);
    void statusChanged(QString);
    void durationChanged(int);
    void albumChanged(QString);
    void trackNumberChanged(int);
    void artistChanged(QString);
    void numberOfTracksChanged(int);
    void genreChanged(QString);
    void shuffleChanged(QString);
    void repeatChanged(QString);
    void positionChanged(int);

private slots:
    void onPropertiesChanged(const QString path,
                             const QMap<QString,QVariant> changedProperties,
                             const QStringList invalidatedProperties);

private:
    QDBusInterface *m_mediaPlayer;
    QString m_status;
    QString m_title;
    int m_duration;
    QString m_album;
    int m_trackNumber;
    QString m_artist;
    int m_numberOfTracks;
    QString m_genre;

    void setStatus(const QString &status);

    void setTitle(const QString &title);
    void setDuration(int duration);
    void setAlbum(const QString &album);
    void setTrackNumber(int trackNumber);
    void setArtist(const QString &artist);
    void setNumberOfTracks(int numberOfTracks);
    void setGenre(const QString &genre);

    QString getTitle();
    QString getStatus();
    int getDuration();
    QString getAlbum();
    int getTrackNumber();
    QString getArtist();
    int getNumberOfTracks();
    QString getGenre();
    QString getShuffle();
    QString getRepeat();
    int getPosition();

    QString getCachedStringProperty(const QString &name);
    int getCachedIntProperty(const QString &name);
    QMap<QString, QVariant> getCachedMapProperty(const QString &name);
    QString getCachedMapPropertyString(const QString &name, const QString &property);
    int getCachedMapPropertyInt(const QString &name, const QString &property);

    QMap<QString, QVariant> m_properties;
};

#endif // BLUEPLAYER_H

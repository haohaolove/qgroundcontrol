/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "LifetimeChecker.h"

#include <QObject>

class AirMapSharedState;

/// class to send telemetry data to AirMap
class AirMapTelemetry : public QObject, public LifetimeChecker
{
    Q_OBJECT
public:
    AirMapTelemetry(AirMapSharedState& shared);
    virtual ~AirMapTelemetry() = default;

    /**
     * Setup the connection to start sending telemetry
     */
    void startTelemetryStream(const QString& flightID);

    void stopTelemetryStream();

    bool isTelemetryStreaming() const;

signals:
    void error(const QString& what, const QString& airmapdMessage, const QString& airmapdDetails);

public slots:
    void vehicleMavlinkMessageReceived(const mavlink_message_t& message);

private:

    void _handleGlobalPositionInt(const mavlink_message_t& message);
    void _handleGPSRawInt(const mavlink_message_t& message);

    enum class State {
        Idle,
        StartCommunication,
        EndCommunication,
        Streaming,
    };

    State                   _state = State::Idle;

    AirMapSharedState&      _shared;
    std::string              _key; ///< key for AES encryption (16 bytes)
    QString                 _flightID;

    float                   _lastHdop = 1.f;
};

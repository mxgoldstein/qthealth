// SPDX-FileCopyrightText: 2022 The Qt Company Ltd.
//
// SPDX-License-Identifier: BSD-3-Clause

#include "qthealthpluginhealthkit.h"

#include <CoreLocation/CoreLocation.h>
#include <HealthKit/HealthKit.h>

#include <QDateTime>
#include <QDebug>

#include <qhealthgeodata.h>
#include <qhealthvaluedata.h>

QtHealthPluginHealthKit::QtHealthPluginHealthKit() {}

QtHealthPluginHealthKit::~QtHealthPluginHealthKit() {
  if (m_healthStore != nullptr)
    [m_healthStore release];
}

void QtHealthPluginHealthKit::initialize() {
  if (![HKHealthStore isHealthDataAvailable])
    return;
  m_healthStore = [[[HKHealthStore alloc] init] retain];
}

bool QtHealthPluginHealthKit::isAvailable() { return m_healthStore != nullptr; }

HKObjectType *getType(QHealthData::Type permission) {
  switch (permission) {
  case QHealthData::Workout:
    return [HKObjectType workoutType];
  case QHealthData::HeartData:
    return [HKObjectType
        quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRate];
  case QHealthData::GeoData:
    return [HKSeriesType workoutRouteType];
  default:
    return nullptr;
  }
}

HKSampleType *getSampleType(QHealthData::Type permission) {
  switch (permission) {
  case QHealthData::Workout:
    return [HKSampleType workoutType];
  case QHealthData::HeartData:
    return [HKSampleType
        quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRate];
  case QHealthData::GeoData:
    return [HKSeriesType workoutRouteType];
  default:
    return nullptr;
  }
}

bool setTypes(NSMutableSet *set, QHealthData::Types permissions) {
  for (const auto &permission :
       {QHealthData::Workout, QHealthData::HeartData, QHealthData::GeoData}) {
    auto *type = getType(permission);
    Q_ASSERT(type != nullptr);
    if (permissions & permission)
      [set addObject:type];
  }

  // There are some permisisons here we do not handle, return false
  if ((permissions & (QHealthData::Workout | QHealthData::HeartData |
                      QHealthData::GeoData)) != permissions)
    return false;

  return true;
}

void QtHealthPluginHealthKit::requestPermissions(
    QHealthData::Types read, QHealthData::Types write,
    std::function<void(bool, QHealthProvider::PermissionError, QString)>
        requestDone) {
  dispatch_async(dispatch_get_main_queue(), ^{
    @autoreleasepool {
      NSMutableSet *readTypes = [[NSMutableSet alloc] init];
      NSMutableSet *writeTypes = [[NSMutableSet alloc] init];

      if (!setTypes(readTypes, read) || !setTypes(writeTypes, write)) {
        requestDone(false, QHealthProvider::PermissionUnsupported, "");
      }

      @try {
        [m_healthStore
            requestAuthorizationToShareTypes:writeTypes
                                   readTypes:readTypes
                                  completion:^(BOOL success,
                                               NSError *_Nullable error) {
                                    requestDone(
                                        success,
                                        success
                                            ? QHealthProvider::NoPermissionError
                                            : QHealthProvider::
                                                  PermissionAPIError,
                                        (!success && error != NULL)
                                            ? QString::fromNSString(
                                                  error.description)
                                            : QString());
                                  }];
      } @catch (NSException *exception) {
        // TODO: Make this more granular
        requestDone(false, QHealthProvider::PermissionAPIError,
                    QString::fromNSString(exception.reason));
      }
    }
  });
}

QHealthProvider::PermissionStatus
QtHealthPluginHealthKit::permission(QHealthData::Type permission,
                                    QHealthProvider::AccessType readWrite) {
  @autoreleasepool {
    if (readWrite == QHealthProvider::Read)
      return QHealthProvider::StatusUnavailable;

    auto *type = getType(permission);
    if (type == nullptr)
      return QHealthProvider::StatusUnavailable;

    HKAuthorizationStatus status =
        [m_healthStore authorizationStatusForType:type];
    switch (status) {
    case HKAuthorizationStatusNotDetermined:
      return QHealthProvider::StatusNotRequested;
    case HKAuthorizationStatusSharingDenied:
      return QHealthProvider::StatusDenied;
    case HKAuthorizationStatusSharingAuthorized:
      return QHealthProvider::StatusGranted;
    }
  }
}

static QHealthWorkout::Type
mapActivityTypeToWorkoutType(HKWorkoutActivityType type) {
  // Not every HKWorkoutActivityType will map to a workout type but we will
  // choose the most appropriate entry Also some types are too obscure to be
  // covered

  // TOOO: Make more comprehensive
  switch (type) {
  case HKWorkoutActivityTypeRunning:
    return QHealthWorkout::Running;
  case HKWorkoutActivityTypeCycling:
    return QHealthWorkout::Cycling;
  case HKWorkoutActivityTypeSwimming:
    return QHealthWorkout::Swimming;
  case HKWorkoutActivityTypeWalking:
    return QHealthWorkout::Walking;
  case HKWorkoutActivityTypeCoreTraining:
    return QHealthWorkout::CoreTraining;
  default:
    return QHealthWorkout::Unknown;
  }
}

static HKWorkoutActivityType
mapWorkoutTypeToActivityType(QHealthWorkout::Type type) {
  // Every QHealthWorkout::Type should map to a HKWorkoutActivityType

  // TOOO: Make more comprehensive
  switch (type) {
  case QHealthWorkout::Running:
    return HKWorkoutActivityTypeRunning;
  case QHealthWorkout::Cycling:
    return HKWorkoutActivityTypeCycling;
  case QHealthWorkout::Swimming:
    return HKWorkoutActivityTypeSwimming;
  case QHealthWorkout::Walking:
    return HKWorkoutActivityTypeWalking;
  case QHealthWorkout::CoreTraining:
    return HKWorkoutActivityTypeCoreTraining;
  case QHealthWorkout::Unknown:
  case QHealthWorkout::Invalid:
    return HKWorkoutActivityTypeOther;
  }
}

static QHealthValue *convertQuantity(HKQuantity *quantity,
                                     HKQuantityType *type) {
  if (type ==
      [HKSampleType
          quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRate]) {
    return new QHealthValue(
        QHealthValue::Heartbeat,
        [quantity doubleValueForUnit:[HKUnit unitFromString:@"count/min"]],
        QHealthValue::BeatsPerMinute);
  } else {
    return NULL;
  }
}

static QHealthData *convertSample(HKSample *sample) {
  if (sample.sampleType == [HKSampleType workoutType]) {
    HKWorkout *hkWorkout = static_cast<HKWorkout *>(sample);

    QHealthWorkout *qtWorkout = new QHealthWorkout;
    qtWorkout->setNativeWorkout(reinterpret_cast<void *>(hkWorkout));

    // TODO: This is horrible. Replace it with something that doesn't require
    // QObjects and doesn't use terrible hacks.
    [hkWorkout retain];
    QObject::connect(qtWorkout, &QObject::destroyed,
                     [hkWorkout] { [hkWorkout release]; });

    qtWorkout->setUuid(QString::fromNSString([sample.UUID UUIDString]));
    qtWorkout->setType(
        mapActivityTypeToWorkoutType(hkWorkout.workoutActivityType));
    qtWorkout->setStartDate(QDateTime::fromNSDate(sample.startDate));
    qtWorkout->setEndDate(QDateTime::fromNSDate(sample.endDate));
    qtWorkout->setDistance(new QHealthValue(
        QHealthValue::Distance,
        [hkWorkout.totalDistance doubleValueForUnit:[HKUnit meterUnit]],
        QHealthValue::Meters));
    qtWorkout->setDuration(new QHealthValue(
        QHealthValue::Duration, hkWorkout.duration, QHealthValue::Seconds));
    qtWorkout->setEnergyBurned(new QHealthValue(
        QHealthValue::Energy,
        [hkWorkout.totalEnergyBurned doubleValueForUnit:[HKUnit jouleUnit]],
        QHealthValue::Joules));

    return qtWorkout;
  } else if (sample.sampleType ==
             [HKSampleType
                 quantityTypeForIdentifier:HKQuantityTypeIdentifierHeartRate]) {

    QHealthValueData *value = new QHealthValueData;
    value->setStartDate(QDateTime::fromNSDate(sample.startDate));
    value->setEndDate(QDateTime::fromNSDate(sample.endDate));

    HKQuantitySample *hkQuantitySample =
        static_cast<HKQuantitySample *>(sample);
    auto *healthValue = convertQuantity(hkQuantitySample.quantity,
                                        hkQuantitySample.quantityType);

    if (healthValue == NULL) {
      value->deleteLater();
      return NULL;
    }

    value->setValue(healthValue);
    return value;
  } else {
    return NULL;
  }
}

void QtHealthPluginHealthKit::executeQuery(
    QHealthQuery *query,
    std::function<void(QHealthQuery *, bool, QList<QHealthData *>,
                       QHealthQuery::Error, QString)>
        queryDone) {
  @autoreleasepool {
    auto *type = getSampleType(query->dataType());
    if (type == nullptr) {
      queryDone(query, false, {}, QHealthQuery::UnsupportedDataType, QString());
      return;
    }

    NSMutableArray *predicates = [[NSMutableArray alloc] init];

    [predicates
        addObject:
            [HKQuery
                predicateForSamplesWithStartDate:(query->startDate().isValid()
                                                      ? query->startDate()
                                                            .toNSDate()
                                                      : [NSDate distantPast])
                                         endDate:(query->endDate().isValid()
                                                      ? query->endDate()
                                                            .toNSDate()
                                                      : [NSDate now])options
                                                :HKQueryOptionNone]];

    if (!query->uuid().isEmpty()) {
      [predicates
          addObject:[HKQuery predicateForObjectWithUUID:
                                 [[NSUUID alloc]
                                     initWithUUIDString:query->uuid()
                                                            .toNSString()]]];
    }

    if (query->workoutType() != QHealthWorkout::Invalid) {
      if (query->dataType() != QHealthData::Workout) {
        queryDone(query, false, {}, QHealthQuery::UnsupportedFilter,
                  u"You can only filter workouts by workout type"_qs);
        return;
      }
      [predicates
          addObject:[HKQuery predicateForWorkoutsWithWorkoutActivityType:
                                 mapWorkoutTypeToActivityType(
                                     query->workoutType())]];
    }

    if (query->workout() != nullptr) {
      HKWorkout *workout =
          reinterpret_cast<HKWorkout *>(query->workout()->nativeWorkout());
      if (workout == nullptr) {
        queryDone(
            query, false, {}, QHealthQuery::UnsupportedFilter,
            u"Querying based on workout currently only works with a native backing workout and not with UUIDs"_qs);
        return;
      }
      [predicates addObject:[HKQuery predicateForObjectsFromWorkout:workout]];
    }

    NSPredicate *predicate =
        [NSCompoundPredicate andPredicateWithSubpredicates:predicates];

    NSMutableArray *sortDescriptors = [[NSMutableArray alloc] init];
    const bool ascending = query->sortOrder() == QHealthQuery::Ascending;

    switch (query->sortBy()) {
    case QHealthQuery::ByDate:
      if (query->dataType() == QHealthData::Workout) {
        [sortDescriptors
            addObject:[NSSortDescriptor sortDescriptorWithKey:@"startDate"
                                                    ascending:ascending]];
      } else {
        queryDone(query, false, {}, QHealthQuery::UnsupportedSortingType,
                  u"Sorting by date is not supported for this type"_qs);
      }
      break;
    case QHealthQuery::ByEnergyBurned:
      if (query->dataType() == QHealthData::Workout) {
        [sortDescriptors
            addObject:[NSSortDescriptor
                          sortDescriptorWithKey:
                              HKWorkoutSortIdentifierTotalEnergyBurned
                                      ascending:ascending]];
      } else {
        queryDone(
            query, false, {}, QHealthQuery::UnsupportedSortingType,
            u"Sorting by energy burned is only supported for workouts"_qs);
      }
      break;
    case QHealthQuery::ByDistance:
      if (query->dataType() == QHealthData::Workout) {
        [sortDescriptors
            addObject:[NSSortDescriptor sortDescriptorWithKey:
                                            HKWorkoutSortIdentifierTotalDistance
                                                    ascending:ascending]];
      } else {
        queryDone(query, false, {}, QHealthQuery::UnsupportedSortingType,
                  u"Sorting by distance is only supported for workouts"_qs);
      }
      break;
    case QHealthQuery::ByDuration:
      if (query->dataType() == QHealthData::Workout) {
        [sortDescriptors
            addObject:[NSSortDescriptor
                          sortDescriptorWithKey:HKWorkoutSortIdentifierDuration
                                      ascending:ascending]];
      } else {
        queryDone(query, false, {}, QHealthQuery::UnsupportedSortingType,
                  u"Sorting by duration is only supported for workouts"_qs);
      }
      break;
    case QHealthQuery::Unsorted:
      break;
    default:
      queryDone(query, false, {}, QHealthQuery::UnsupportedSortingType,
                QString());
      return;
    }

    HKQuery *hkQuery = [[HKSampleQuery alloc]
        initWithSampleType:type
                 predicate:predicate
                     limit:(query->limit() == QHealthQuery::NoLimit
                                ? HKObjectQueryNoLimit
                                : query->limit())
           sortDescriptors:sortDescriptors
            resultsHandler:^(HKSampleQuery *_Nonnull hkQuery,
                             NSArray<__kindof HKSample *> *_Nullable results,
                             NSError *_Nullable error) {
              if (results == NULL) {
                queryDone(query, false, {}, QHealthQuery::APIError,
                          (error != NULL)
                              ? QString::fromNSString(error.description)
                              : QString());
                return;
              }

              if (query->dataType() == QHealthData::GeoData) {
                auto routeCount = [results count];
                if (routeCount == 0) {
                  queryDone(query, true, {}, QHealthQuery::NoError, QString());
                  return;
                }

                if (routeCount > 1) {
                  queryDone(
                      query, false, {}, QHealthQuery::UnsupportedDataType,
                      u"We do not support processing multiple workout routes at the same time"_qs);
                  return;
                }

                HKWorkoutRoute *route =
                    static_cast<HKWorkoutRoute *>(results[0]);

                NSMutableArray *locations =
                    [[[NSMutableArray alloc] init] retain];

                @try {
                  HKQuery *workoutRouteQuery = [[HKWorkoutRouteQuery alloc]
                      initWithRoute:route
                        dataHandler:^(HKWorkoutRouteQuery *hkQuery,
                                      NSArray<CLLocation *> *routeData,
                                      BOOL done, NSError *error) {
                          if (results == NULL) {
                            queryDone(query, false, {}, QHealthQuery::APIError,
                                      (error != NULL) ? QString::fromNSString(
                                                            error.description)
                                                      : QString());
                            return;
                          }

                          [locations addObjectsFromArray:routeData];
                          if (done) {
                            QList<QHealthData *> qtResults;
                            for (CLLocation *location in locations) {
                              QHealthGeoData *geoData = new QHealthGeoData;
                              geoData->setAltitude(location.altitude);
                              geoData->setLongitude(
                                  location.coordinate.longitude);
                              geoData->setLatitude(
                                  location.coordinate.latitude);
                              QDateTime date =
                                  QDateTime::fromNSDate(location.timestamp);
                              geoData->setStartDate(date);
                              geoData->setEndDate(date);

                              qtResults << geoData;
                            }
                            queryDone(query, true, qtResults,
                                      QHealthQuery::NoError, QString());
                            [locations release];
                          }
                        }];

                  [m_healthStore executeQuery:workoutRouteQuery];
                  qDebug() << "Done with sending the query";
                } @catch (NSException *exception) {
                  // TODO: Make this more granular
                  queryDone(query, false, {}, QHealthQuery::APIError,
                            QString::fromNSString(exception.reason));
                }
                return;
              }

              dispatch_async(dispatch_get_main_queue(), ^{
                QList<QHealthData *> qtResults;

                for (HKSample *sample in results) {
                  auto *data = convertSample(sample);
                  if (data == nullptr) {
                    queryDone(query, false, {},
                              QHealthQuery::UnsupportedDataType, QString());
                    return;
                  }
                  qtResults << data;
                }

                queryDone(query, true, qtResults, QHealthQuery::NoError,
                          QString());
              });
            }];
    [m_healthStore executeQuery:hkQuery];
  }
}

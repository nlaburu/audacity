/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "../../itrackeditinteraction.h"

#include "Track.h"
#include "iselectioncontroller.h"
#include "modularity/ioc.h"
#include "context/iglobalcontext.h"

class AudacityProject;

struct TrackData
{
    // Track type from Track.h
    std::shared_ptr<Track> track;
    au::trackedit::ClipKey clipKey;
};

struct Clipboard
{
    std::vector<TrackData> data;
};

namespace au::trackedit {
class Au3Interaction : public ITrackeditInteraction
{
    muse::Inject<au::context::IGlobalContext> globalContext;
    muse::Inject<au::trackedit::ISelectionController> selectionController;

public:
    Au3Interaction() = default;

    audio::secs_t clipStartTime(const trackedit::ClipKey& clipKey) const override;

    bool changeClipStartTime(const trackedit::ClipKey& clipKey, secs_t newStartTime, bool completed) override;
    muse::async::Channel<trackedit::ClipKey, secs_t /*newStartTime*/, bool /*completed*/> clipStartTimeChanged() const override;

    bool trimTrackData(trackedit::TrackId trackId, secs_t begin, secs_t end) override;
    bool silenceTrackData(trackedit::TrackId trackId, secs_t begin, secs_t end) override;

    bool changeClipTitle(const trackedit::ClipKey& clipKey, const muse::String& newTitle) override;
    void clearClipboard() override;
    bool pasteFromClipboard(secs_t begin, trackedit::TrackId trackId) override;
    bool copyClipIntoClipboard(const trackedit::ClipKey& clipKey) override;
    bool copyClipDataIntoClipboard(const trackedit::ClipKey& clipKey, secs_t begin, secs_t end) override;
    bool copyTrackDataIntoClipboard(const trackedit::TrackId trackId, secs_t begin, secs_t end) override;
    bool removeClip(const trackedit::ClipKey& clipKey) override;
    bool removeClipData(const trackedit::ClipKey& clipKey, secs_t begin, secs_t end) override;
    bool splitAt(const TrackId trackId, secs_t pivot) override;
    bool mergeSelectedOnTracks(const std::vector<TrackId> tracksIds, secs_t begin, secs_t end) override;
    bool trimClipLeft(const trackedit::ClipKey& clipKey, secs_t deltaSec) override;
    bool trimClipRight(const trackedit::ClipKey& clipKey, secs_t deltaSec) override;
    void newMonoTrack() override;
    void newStereoTrack() override;
    void newLabelTrack() override;
    audio::secs_t clipDuration(const trackedit::ClipKey& clipKey) const override;

private:
    AudacityProject& projectRef() const;
    bool pasteIntoNewTrack();
    ::Track::Holder createNewTrackAndPaste(std::shared_ptr<::Track> data, ::TrackList &list, secs_t begin);
    std::vector<TrackId> determineDestinationTracksIds(const std::vector<Track>& tracks,
                                    TrackId destinationTrackId, size_t tracksNum) const;
    bool canPasteClips(const std::vector<TrackId>& tracksIds,  secs_t begin) const;
    bool mergeSelectedOnTrack(const TrackId trackId, secs_t begin, secs_t end);

    void pushProjectHistoryJoinState(secs_t start, secs_t duration);

    muse::async::Channel<trackedit::ClipKey, secs_t /*newStartTime*/, bool /*completed*/> m_clipStartTimeChanged;

    static Clipboard s_clipboard;
};
}